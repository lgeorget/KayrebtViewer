#include <QString>
#include <QFileDialog>
#include <QGraphicsView>
#include "viewer.h"
#include "drawing.h"
#include "ui_viewer.h"

Viewer::Viewer(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Viewer)
{
	ui->setupUi(this);

	QSettings settings;
	_srcTree = settings.value("source tree").toString();
	_symbDb = settings.value("symbol database").toString();
	_diagdir = settings.value("diagrams dir").toString();

	// TODO
/*	_configDock = new QDockWidget(this);
	addDockWidget(Qt::RightDockWidgetArea, _configDock);
	_configDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
*/
	_sourcesDock = new QDockWidget(this);
	addDockWidget(Qt::RightDockWidgetArea, _sourcesDock);
	_sourcesDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

	_databaseDock = new QDockWidget(this);
	addDockWidget(Qt::LeftDockWidgetArea, _databaseDock);
	_databaseDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

	_srcTreeWidget = new SourceTreeWidget();
	_sourcesDock->setWidget(_srcTreeWidget);
	_dbviewer = new DatabaseViewer();
	_databaseDock->setWidget(_dbviewer);

	connect(ui->actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui->actionOuvrir, SIGNAL(triggered()), this, SLOT(openGraph()));
	connect(_dbviewer, SIGNAL(symbolSelected(QString)), this, SLOT(openGraph(QString)));
	connect(_dbviewer, SIGNAL(fileSelected(QString)), _srcTreeWidget, SLOT(selectFile(QString)));
	connect(_srcTreeWidget, SIGNAL(filenameSelected(QString,QString)), _dbviewer, SLOT(selectFileAndDirectory(QString,QString)));
}

void Viewer::openGraph()
{
	openGraph(QFileDialog::getOpenFileName(this, tr("Select the diagram to open"), _diagdir,
			  "GraphViz files (*.dot)"));
}

void Viewer::openGraph(QString filename)
{
	qDebug() << "About to open: " << filename;
	if (!filename.isEmpty()) {
		if (!QFile(filename).exists()) {
			QMessageBox::critical(this, tr("Kayrebt::Viewer"), tr("The file you have selected does not exist."));
			return;
		}

		try {
			bool found = false;
			QMdiSubWindow* subw;
			for (int i = 0 ; i < ui->docs->subWindowList().size() && !found ; ++i ) {
				subw = ui->docs->subWindowList()[i];
				if (subw->windowTitle() == filename)
					found = true;
			}

			if (!found) {

				subw = ui->docs->addSubWindow(new Drawing(filename));
				subw->setWindowTitle(QFileInfo(filename).canonicalFilePath());
			}

			ui->docs->setActiveSubWindow(subw);
			subw->show();
		} catch (std::runtime_error& e) {
			QMessageBox::critical(this, tr("Kayrebt::Viewer"), tr("The file you have selected is not a valid GraphViz file.\n\n") + e.what());
		}
	}
}

Viewer::~Viewer()
{
	delete ui;
}
