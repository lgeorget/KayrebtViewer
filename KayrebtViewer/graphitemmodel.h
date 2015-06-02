#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include "databaseviewer.h"

class GraphItemModel : public QAbstractTableModel
{
public:
	explicit GraphItemModel(QObject *parent = 0);
	~GraphItemModel();

	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public slots:

private:
	GraphItem* _rootItem;

};

#endif // HISTORYMODEL_H
