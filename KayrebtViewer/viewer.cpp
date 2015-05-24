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
	_configDock = new QDockWidget(this);
	addDockWidget(Qt::RightDockWidgetArea, _configDock);
	_configDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

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
	openGraph(QFileDialog::getOpenFileName(this, tr("Select the diagram to open"), QDir::currentPath(),
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
				subw->setWindowTitle(filename);
			}

			ui->docs->setActiveSubWindow(subw);
			subw->show();
		} catch (std::runtime_error& e) {
			QMessageBox::critical(this, tr("Kayrebt::Viewer"), tr("The file you have selected is not a valid GraphViz file.\n\n"
																  "The error received from GraphViz is: ") + e.what());
		}
	}
}

Viewer::~Viewer()
{
	delete ui;
}
