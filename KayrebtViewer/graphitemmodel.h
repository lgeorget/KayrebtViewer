#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QVariant>
#include <limits>
#include "databaseviewer.h"

class GraphItemModel : public QAbstractItemModel
{
public:
	explicit GraphItemModel(QObject *parent = 0);
	~GraphItemModel();

	int rowCount(const QModelIndex & parent = QModelIndex()) const override;
	int columnCount(const QModelIndex & parent = QModelIndex()) const override;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex() ) override;
	bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;

	QModelIndex findGraphItem(quint64 id) const;

	bool appendGraphItem(const GraphItem& newItem, const QModelIndex& parent = QModelIndex(), int pos = std::numeric_limits<int>::max());

public slots:

private:
	GraphItem* _rootItem;
	GraphItem* getItem(const QModelIndex& index) const;

};

#endif // HISTORYMODEL_H
