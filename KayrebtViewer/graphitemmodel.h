/**
 * @file graphitemmodel.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class GraphItemModel
 */
#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QVariant>
#include <limits>
#include "databaseviewer.h"

/**
 * \brief This class represents a collection of GraphItem.
 *
 * It can be used with any view respecting the interface designed by Qt and is
 * specifically suited for tree views.
 */
class GraphItemModel : public QAbstractItemModel
{
public:
	/**
	 * \brief Constructs an empty model.
	 *
	 * \param parent the parent object of this model
	 */
	explicit GraphItemModel(QObject *parent = 0);
	/**
	 * \brief Destroys the model and every items it owns
	 */
	~GraphItemModel();

	/**
	 * \brief Gives the number of rows for an item.
	 *
	 * Reimplemented from QAbstractItemModel.
	 *
	 * \param parent the index of the item of interest. If this index is
	 * invalid, this method returns the number of top-level items in the
	 * model. Otherwise, it returns the number of children for the item.
	 *
	 * \return the number of rows of a given item
	 */
	int rowCount(const QModelIndex & parent = QModelIndex()) const override;
	/**
	 * \brief Gives the number of columns in the model.
	 *
	 * Reimplemented from QAbstractItemModel. The number of columns is the
	 * number of fields in a GraphItem objects. This is a fixed value.
	 *
	 * \param parent unused parameter
	 *
	 * \return the number of columns in the model
	 */
	int columnCount(const QModelIndex & parent = QModelIndex()) const override;
	/**
	 * \brief Retrieves the data attached to index \p index in the model.
	 *
	 * Reimplemented from QAbstractItemModel.
	 *
	 * \param index the field of the item of interest
	 * (parent item, row = child index, column = field)
	 * \param role the role the data has (display, quick help, etc.)
	 *
	 * \return the data contained at index \p index
	 */
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
	/**
	 * \brief Retrieves the text for the columns headers.
	 *
	 * \param section the column index
	 * \param orientation should be Qt::Horizontal, as the vertical header
	 * is not set by this model
	 * \param role the role the data has (display, quick help, etc.)
	 *
	 * \return the text for the header
	 */
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	/**
	 * \brief Insert new rows in the model, populated with empty GraphItem-s.
	 *
	 * Reimplemented from QAbstractGraphItem.
	 *
	 * \param row the row after which new rows will be inserted
	 * \param count the number of new rows to insert
	 * \param parent the parent item which will have new children
	 *
	 * \return true if, and only if, the insertion takes place (row and
	 * count must be sensible values)
	 */
	bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;
	/**
	 * \brief Remove rows from the model.
	 *
	 * Reimplemented from QAbstractGraphItem.
	 *
	 * \param row the index from which rows will be removed
	 * \param count the number of new rows to remove
	 * \param parent the parent item from which items are removed
	 *
	 * \return true if, and only if, the removal takes place (row and
	 * count must be sensible values)
	 */
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex() ) override;
	/**
	 * \brief Set the value of a field of a item in the model.
	 *
	 * Reimplemented from QAbstractGraphItem.
	 *
	 * \param index the item and field of interest
	 * (parent item, row = child index, column = field)
	 * \param value the new value for the field
	 * \param role the role of the data (display, quick help, etc.)
	 *
	 * \return true if, and only if, the new data could be set
	 */
	bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
	/**
	 * \brief Builds an index for an item in the model.
	 *
	 * Reimplemented from QAbstractGraphItem.
	 *
	 * \param row the row of the item, in its parent's list
	 * \param column the field (0 if the index is for the entire line)
	 * \param parent the parent index
	 *
	 * \return an index for the item, possibly invalid if incorrect values
	 * were passed as parameters
	 */
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	/**
	 * \brief Gives the index of the parent of an item.
	 *
	 * Reimplemented from QAbstractGraphItem.
	 *
	 * \param index the item of interest
	 *
	 * \return the index for the \p index 's parent, or an invalid index if
	 * it has no parent
	 */
	QModelIndex parent(const QModelIndex &index) const override;

	/**
	 * \brief Retrieves the index of an item of given identifier.
	 *
	 * \param id the identifier to look for in the model
	 *
	 * \return the index for the item if it was found, or an invalid index
	 * otherwise
	 */
	QModelIndex findGraphItem(quint64 id) const;

	/**
	 * \brief Adds a GraphItem to the model.
	 *
	 * \param newItem the new GraphItem to add
	 * \param parent the parent the new item will have in the model
	 * \param pos the position in the \p parent children list at which the
	 * item is inserted
	 *
	 * \return true if, and only if, the GraphITem could be added
	 */
	bool appendGraphItem(const GraphItem& newItem, const QModelIndex& parent = QModelIndex(), int pos = std::numeric_limits<int>::max());

public slots:

private:
	/**
	 * \brief the root, virtual, item
	 */
	GraphItem* _rootItem;
	/**
	 * \brief Gets a GraphItem from its index
	 *
	 * \param index the item of interest
	 *
	 * \return a pointer to a GraphItem with index \p index
	 */
	GraphItem* getItem(const QModelIndex& index) const;

};

#endif // HISTORYMODEL_H
