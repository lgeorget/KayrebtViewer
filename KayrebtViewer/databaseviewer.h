#ifndef DATABASEVIEWER_H
#define DATABASEVIEWER_H

#include <QtGui>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include "databasesortfilterproxymodel.h"

class DatabaseViewer : public QWidget
{
	Q_OBJECT
public:
	explicit DatabaseViewer(QWidget *parent = 0);
	~DatabaseViewer();

signals:
	void symbolSelected(QString symbol);
	void fileSelected(QString symbol);

public slots:
	void selectFileAndDirectory(QString dir, QString file);

private:
	QTableView* _dbView;
	QSqlTableModel* _db;
	QSqlDatabase _dbBackend;
	DatabaseSortFilterProxyModel* _dbFilter;

	QLineEdit* _filterName;
	QLineEdit* _filterDir;
	QLineEdit* _filterFile;

private slots:
	void symbolDoubleClicked(const QModelIndex& index);
};

#endif // DATABASEVIEWER_H
