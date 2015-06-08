#ifndef DATABASEVIEWER_H
#define DATABASEVIEWER_H

#include <QtGui>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include "databasesortfilterproxymodel.h"

namespace Ui {
  class DatabaseViewer;
}

class GraphItem;
class GraphItemModel;

class DatabaseViewer : public QTabWidget
{
	Q_OBJECT

public:
	explicit DatabaseViewer(GraphItemModel* history, QWidget *parent = 0);
	~DatabaseViewer();

signals:
	void graphSelected(QString graph);
	void fileSelected(QString symbol);

public slots:
	void selectFileAndDirectory(QString dir, QString file);
	void addGraphToHistory(const GraphItem &graph);
	void showHistoryContextMenu(const QPoint& point);

private:
	Ui::DatabaseViewer *_ui;
	QSqlTableModel* _db;
	QSqlDatabase _dbBackend;
	DatabaseSortFilterProxyModel* _dbFilter;
	GraphItemModel* _openGraphs;
	void symbolDoubleClicked(const QAbstractItemModel *model, const QModelIndex& index);

private slots:
	void databaseSymbolDoubleClicked(const QModelIndex& index);
	void historySymbolDoubleClicked(const QModelIndex& index);
};

#endif // DATABASEVIEWER_H
