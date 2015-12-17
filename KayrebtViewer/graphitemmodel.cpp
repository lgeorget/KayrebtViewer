/**
 * @file graphitemmodel.cpp
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Implementation of class GraphItemModel
 */
#include <QtGui>
#include <tuple>
#include <limits>
#include "graphitemmodel.h"
#include "graphitem.h"

GraphItemModel::GraphItemModel(QObject *parent) :
	QAbstractItemModel(parent),
	_rootItem(new GraphItem())
{}

GraphItemModel::~GraphItemModel()
{
	delete _rootItem;
}

int GraphItemModel::rowCount(const QModelIndex & parent) const
{
	GraphItem* item = getItem(parent);
	return item->childrenCount();
}

int GraphItemModel::columnCount(const QModelIndex &) const
{
	return GraphItem::columnCount();
}

QVariant GraphItemModel::data(const QModelIndex & index, int role) const
{
	if (index.isValid()) {
		if (role == Qt::DisplayRole) {
			return getItem(index)->getValue(index.column());
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

bool GraphItemModel::insertRows( int row, int count, const QModelIndex & parent)
{
	GraphItem* parentItem = getItem(parent);

	if (row < 0 || count < 0)
		return false;

	beginInsertRows(parent, row, row+count);
	for (int i = row ; i < row+count ; i++)
		parentItem->addChild(i, GraphItem());
	endInsertRows();

	return true;
}

bool GraphItemModel::removeRows(int row, int count, const QModelIndex & parent)
{
	GraphItem* parentItem = getItem(parent);

	if (parentItem->childrenCount() < row + count - 1)
		return false; //not enough elements present in the model

	beginRemoveRows(parent, row, row + count - 1);
	for (int i = row ; i < row + count ; i++)
		parentItem->removeChild(i);
	endRemoveRows();

	return true;
}

bool GraphItemModel::setData(const QModelIndex& index, const QVariant & value, int role)
{
	if (!index.isValid())
		return false;

	if (role != Qt::EditRole)
		return false;

	GraphItem* parentItem = getItem(index);
	bool ok = parentItem->setValue(index.column(), value);

	if (ok)
		emit dataChanged(index, index);

	return ok;
}


bool GraphItemModel::appendGraphItem(const GraphItem &newItem, const QModelIndex& parent, int pos)
{
	GraphItem* parentItem = getItem(parent);

	if (pos > parentItem->childrenCount())
		pos =  parentItem->childrenCount();

	beginInsertRows(parent, pos, pos);
	parentItem->addChild(pos, newItem);
	endInsertRows();

	qDebug() << parentItem << " " << parentItem->childrenCount() << " " << parentItem->getId();
	return true;
}

QModelIndex GraphItemModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid() && parent.column() != 0) //only column 0 hosts children
		return QModelIndex();

	GraphItem* parentItem = getItem(parent);
	if (parentItem->childrenCount() > row)
		return createIndex(row, column, parentItem->child(row));

	return QModelIndex();
}

QModelIndex GraphItemModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	GraphItem* item = getItem(index);
	GraphItem* parent = item->getParent();
	if (!parent)
		return QModelIndex();

	return createIndex(item->childNumber(), 0, parent);
}

QModelIndex GraphItemModel::findGraphItem(quint64 id) const
{
	if (id == 0)
		return QModelIndex();

	QList<GraphItem*> items;
	items << _rootItem;

	bool found = false;
	int i;
	GraphItem* item;
	while (!items.isEmpty() && !found) {
		item = items[0];
		items.removeFirst();
		if (item->getId() == id) {
			found = true;
		} else {
			for (i=0 ; i < item->childrenCount() && !found ; i++) {
				items << item->child(i);
			}
		}
	}
	if (found)
		return createIndex(item->childNumber(), 0, item);

	return QModelIndex();
}

GraphItem* GraphItemModel::getItem(const QModelIndex &index) const
{
	if (index.isValid()) {
		GraphItem* item = reinterpret_cast<GraphItem*>(index.internalPointer());
		if (item)
			return item;
	}
	return _rootItem;
}
