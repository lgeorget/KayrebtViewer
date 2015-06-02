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

class HistoryModel;

class DatabaseViewer : public QTabWidget
{
	Q_OBJECT

public:
	struct RowContent {
		QString symbol;
		QString dir;
		QString file;

		QVariant get(int i) const {
			switch (i) {
				case 0:
					return symbol;
				case 1:
					return dir;
				case 2:
					return file;
				default:
					return QVariant();
			}
		}
	};

	explicit DatabaseViewer(QWidget *parent = 0);
	~DatabaseViewer();

signals:
	void symbolSelected(QString symbol);
	void fileSelected(QString symbol);

public slots:
	void selectFileAndDirectory(QString dir, QString file);
	void addGraphToHistory(const QFileInfo& graph);

private:
	Ui::DatabaseViewer *_ui;
	QSqlTableModel* _db;
	QSqlDatabase _dbBackend;
	DatabaseSortFilterProxyModel* _dbFilter;
	HistoryModel* _openGraphs;
	void symbolDoubleClicked(const QAbstractItemModel *model, const QModelIndex& index);

private slots:
	void databaseSymbolDoubleClicked(const QModelIndex& index);
	void historySymbolDoubleClicked(const QModelIndex& index);
};

#endif // DATABASEVIEWER_H
