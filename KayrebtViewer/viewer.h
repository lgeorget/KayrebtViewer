#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QListWidget>
#include <QtGui>
#include "graph.h"
#include "sourcetreewidget.h"
#include "databaseviewer.h"

namespace Ui {
  class Viewer;
}

class Viewer : public QMainWindow
{
  Q_OBJECT

public:
  explicit Viewer(QWidget *parent = 0);
  ~Viewer();

public slots:
  void openGraph();
  void openGraph(QString filename);

private:
  Ui::Viewer *ui;
  QDockWidget* _configDock;
  QDockWidget* _sourcesDock;
  QDockWidget* _databaseDock;
  SourceTreeWidget* _srcTreeWidget;
  DatabaseViewer* _dbviewer;
//  QList<Drawing *> _scenes;


};

#endif // VIEWER_H
