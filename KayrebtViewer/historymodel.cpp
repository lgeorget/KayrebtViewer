#include <QtGui>
#include "historymodel.h"

HistoryModel::HistoryModel(QAbstractTableModel* sourceDb, QObject *parent) :
	QAbstractTableModel(parent),
	_sourceDb(sourceDb)
{}

int HistoryModel::rowCount(const QModelIndex &) const
{
	return _sourceRows.count();
}

int HistoryModel::columnCount(const QModelIndex & parent) const
{
	return _sourceDb->columnCount(parent);
}

QVariant HistoryModel::data(const QModelIndex & index, int role) const
{
	if (index.isValid()) {
		if (role == Qt::DisplayRole) {
			return _sourceRows.at(index.row()).get(index.column());
		}
	}
	return QVariant();
}

QVariant HistoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return _sourceDb->headerData(section, orientation, role);
}

bool HistoryModel::appendRow(const RowContent& newRow, const QModelIndex& parent)
{
	beginInsertRows(parent, _sourceRows.count(), _sourceRows.count());
	_sourceRows.append(newRow);
	endInsertRows();
	return true;
}

bool HistoryModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (row < 0 || row >= _sourceRows.count() || count <= 0)
		return false;

	int realCount = qMax(row+count,_sourceRows.count()+1);
	beginRemoveRows(parent, row, realCount);
	auto startPos = _sourceRows.begin() + row;
	_sourceRows.erase(startPos, startPos+realCount);
	endRemoveRows();
	return true;
}

