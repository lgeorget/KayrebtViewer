#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QListWidget>
#include <QtGui>
#include "graph.h"
#include "sourcetreewidget.h"
#include "databaseviewer.h"
#include "graphitemmodel.h"

namespace Ui {
	class Viewer;
}

class GraphItem;

class Viewer : public QMainWindow
{
	Q_OBJECT

public:
	explicit Viewer(QWidget *parent = 0);
	~Viewer();

	public slots:
	void openGraph();
	void openGraph(const QString &filename);

signals:
	void newGraphOpen(const GraphItem& file);

private:
	Ui::Viewer *ui;
	QDockWidget* _configDock;
	QDockWidget* _sourcesDock;
	QDockWidget* _databaseDock;
	SourceTreeWidget* _srcTreeWidget;
	DatabaseViewer* _dbviewer;
	//  QList<Drawing *> _scenes;

	QString _srcTree;
	QString _symbDb;
	QString _diagdir;
	GraphItemModel _openGraphs;

	static quint64 _graphsIdGenerator;

	quint64 doOpenGraph(const QFileInfo &file);

protected:
	virtual bool event(QEvent *event);
};

#endif // VIEWER_H
