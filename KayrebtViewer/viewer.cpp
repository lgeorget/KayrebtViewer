/**
 * @file viewer.cpp
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class Viewer
 */
#include <QString>
#include <QFileDialog>
#include <QGraphicsView>
#include "viewer.h"
#include "drawing.h"
#include "ui_viewer.h"
#include "hyperlinkactivatedevent.h"
#include "nodehoverevent.h"
#include "graphitem.h"
#include "graphitemmodel.h"
#include "sourcetextviewer.h"

quint64 Viewer::_graphsIdGenerator = 1;

Viewer::Viewer(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Viewer)
{
	ui->setupUi(this);

	QSettings settings;
	_srcTree = settings.value("source tree").toString();
	_symbDb = settings.value("symbol database").toString();
	_diagdir = settings.value("diagrams dir").toString();

	_databaseDock = new QDockWidget(this);
	addDockWidget(Qt::LeftDockWidgetArea, _databaseDock);
	_databaseDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

	_sourcesDock = new QDockWidget(this);
	addDockWidget(Qt::LeftDockWidgetArea, _sourcesDock);
	_sourcesDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

	_srcTreeWidget = new SourceTreeWidget(_sourcesDock);
	_sourcesDock->setWidget(_srcTreeWidget);
	_dbviewer = new DatabaseViewer(&_openGraphs, _databaseDock);
	_databaseDock->setWidget(_dbviewer);

	ui->sources->setFixedWidth(0);

	connect(ui->actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui->actionOuvrir, SIGNAL(triggered()), this, SLOT(openGraph()));
	connect(_dbviewer, SIGNAL(graphSelected(QString)), this, SLOT(openGraph(QString)));
//	connect(_dbviewer, SIGNAL(fileSelected(QString)), ui->sources, SLOT(openSourceFile(QString)));
	connect(ui->docs, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(openSourceFile(QMdiSubWindow*)));
	connect(_dbviewer, SIGNAL(fileSelected(QString)), _srcTreeWidget, SLOT(selectFile(QString)));
	connect(_srcTreeWidget, SIGNAL(filenameSelected(QString,QString)), _dbviewer, SLOT(selectFileAndDirectory(QString,QString)));
//	connect(this, SIGNAL(newGraphOpen(GraphItem)), _dbviewer, SLOT(addGraphToHistory(GraphItem)));
}

void Viewer::openGraph()
{
	openGraph(QFileDialog::getOpenFileName(this, tr("Select the diagram to open"), _diagdir,
			  "GraphViz files (*.dot)"));
}

void Viewer::openSourceFile(QMdiSubWindow* window)
{
	if (window) {
		Drawing* d = static_cast<Drawing*>(window->widget());
		QString srcFilename = _srcTree + d->getGraph()->getSourceFilename();
		ui->sources->openSourceFile(srcFilename);
		// ui->sources->gotoLine(d->getGraph()->getSourceLine()); // implicitly done by highlightLines below
		ui->sources->highlightLines(d->getGraph()->getSourceLine()-1,d->getGraph()->getSourceLine());
	} else if (ui->docs->subWindowList().isEmpty()) {
		ui->sources->clear();
	}
}

void Viewer::openGraph(const QString& filename)
{
	qDebug() << "About to open: " << filename;
	if (!filename.isEmpty()) {
		QFileInfo file(filename);
		if (!file.exists()) {
			QMessageBox::critical(this, tr("Kayrebt::Viewer"), tr("The file you have selected does not exist."));
			return;
		}
		try {
			quint64 id = doOpenGraph(file);
			if (id > 0)
				_openGraphs.appendGraphItem(GraphItem(file, id)); // new root in history
		} catch (std::runtime_error& e) {
			QMessageBox::critical(this, tr("Kayrebt::Viewer"), tr("The file you have selected is not a valid GraphViz file.\n\n") + e.what());
		}
	}
}

quint64 Viewer::doOpenGraph(const QFileInfo& file)
{
	QString newFileName(file.canonicalFilePath());
	quint64 id = 0;

	bool found = false;
	QMdiSubWindow* subw;
	for (int i = 0 ; i < ui->docs->subWindowList().size() && !found ; ++i ) {
		subw = ui->docs->subWindowList()[i];
		if (subw->windowTitle() == file.baseName()) {
			found = true;
		}
	}

	if (!found) {
		id = _graphsIdGenerator++;
		Drawing *d = new Drawing(id, newFileName, this);
		subw = ui->docs->addSubWindow(d);
		subw->setWindowTitle(file.baseName());
	}

	ui->docs->setActiveSubWindow(subw);
	subw->show();

	return id;
}

bool Viewer::event(QEvent *event)
{
	if (event->type() == HyperlinkActivatedEvent::HYPERLINK_ACTIVATED_EVENT) {
		HyperlinkActivatedEvent* realEvent = static_cast<HyperlinkActivatedEvent*>(event);
		QFileInfo file(realEvent->getUrl());
		if (!file.exists()) {
			QMessageBox::critical(this, tr("Kayrebt::Viewer"), tr("The diagram you want does not exist.\n"
																  "Perhaps it is a compiler-generated symbol?\n"));
		} else {
			quint64 id = doOpenGraph(file);
			if (id > 0) {
				QModelIndex index = _openGraphs.findGraphItem(realEvent->getId());
				_openGraphs.appendGraphItem(GraphItem(file, id), index);
			}
		}

		event->accept();
		return true;
	} else if (event->type() == NodeHoverEvent::NODE_HOVER_EVENT) {
		NodeHoverEvent* realEvent = static_cast<NodeHoverEvent*>(event);
		ui->sources->highlightLines(realEvent->getLineNumber(), realEvent->getLineNumber(), false);
	} else {
		return QMainWindow::event(event);
	}
}

Viewer::~Viewer()
{
	delete ui;
}
