#include <QtGui>
#include <tuple>
#include "graphitemmodel.h"
#include "graphitem.h"

GraphItemModel::GraphItemModel(QObject *parent) :
	QAbstractTableModel(parent),
	_rootItem(new GraphItem())
{}

GraphItemModel::~GraphItemModel()
{
	delete _rootItem;
}

int GraphItemModel::rowCount(const QModelIndex & parent) const
{
	if (parent.isValid()) {
		GraphItem* item = reinterpret_cast<GraphItem*>(parent.internalPointer());
		return item->childrenCount();
	} else {
		return _rootItem->childrenCount();
	}
}

int GraphItemModel::columnCount(const QModelIndex &) const
{
	return GraphItem::columnCount();
}

QVariant GraphItemModel::data(const QModelIndex & index, int role) const
{
	if (index.isValid()) {
		if (role == Qt::DisplayRole) {
			return reinterpret_cast<GraphItem*>(index.internalPointer())->child(index.row())->getValue(index.column());
		}
	}
	return QVariant();
}

QVariant GraphItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section >= 0 && section < GraphItem::columnCount()) {
			return GraphItem::COLUMNS[section];
	}
	return QVariant();
}
