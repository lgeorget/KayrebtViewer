#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include "databaseviewer.h"

class HistoryModel : public QAbstractTableModel
{
private:
	using RowContent = DatabaseViewer::RowContent;

public:
	explicit HistoryModel(QAbstractTableModel* sourceDb, QObject *parent = 0);

	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public slots:
	bool appendRow(const RowContent& newRow, const QModelIndex& parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

private:
	QAbstractTableModel* _sourceDb;
	QList<DatabaseViewer::RowContent> _sourceRows;

};

#endif // HISTORYMODEL_H
