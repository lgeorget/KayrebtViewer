/**
 * @file graphitem.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class GraphItem
 */
#ifndef GRAPHITEM_H
#define GRAPHITEM_H

#include <QString>
#include <QFileInfo>
#include <tuple>

class Graph;

/**
 * @brief This class represents an item representing a diagram which can be
 * stored in a Qt model class.
 *
 * GraphItem instances are particularly suited for GraphItemModel Be aware that
 * they do not inherit from any Qt class, including QObject.
 *
 * GraphItems carry an identifier. Identifiers are used to bind items to open
 * diagrams windows. This is necessary to process some events. For example,
 * double-clicking on an item will place the window containing the diagram
 * in the foreground if it is open, or open it if it is not.
 */
class GraphItem
{
public:
	/**
	 * @brief Constructs a GraphItem from an existing graph.
	 *
	 * @param graph the graph of interest
	 * @param id the id of the item (which could be the id of an existing
	 * Drawing too, for cross-referencing)
	 * @param parent the parent item, or an invalid parent if this GraphItem
	 * is a top element
	 */
	explicit GraphItem(const Graph& graph, quint64 id, GraphItem *parent = 0);
	/**
	 * @brief Constructs a GraphItem from a file name
	 *
	 * The file @p graph should exist and contain a GraphViz graph but no
	 * checks is actually made in this class.
	 *
	 * @param graph the file of interest
	 * @param id the id of the item (which could be the id of an existing
	 * Drawing too, for cross-referencing)
	 * @param parent the parent item, or an invalid parent if this GraphItem
	 * is a top element
	 */
	explicit GraphItem(const QFileInfo& graph, quint64 id, GraphItem *parent = 0);
	/**
	 * @brief Copy constructor.
	 *
	 * @param graphItem the original GraphItem
	 */
	GraphItem(const GraphItem& graphItem);
	/**
	 * @brief Constructs an empty, invalid, GraphItem.
	 */
	GraphItem() = default;
	/**
	 * @brief Destroys the GraphItem and all its children.
	 */
	~GraphItem();

	/**
	 * @brief Gets the value of the "Symbol" field of the item.
	 *
	 * @return the value of the "Symbol" field
	 */
	QString getSymbol() const;
	/**
	 * @brief Gets the value of the "Dir" field of the item.
	 *
	 * @return the value of the "Dir" field
	 */
	QString getDir() const;
	/**
	 * @brief Gets the value of the "File" field of the item.
	 *
	 * @return the value of the "File" field
	 */
	QString getFile() const;
	/**
	 * @brief Gives the identifier of the item.
	 *
	 * @return the identifier carried by the item
	 */
	quint64 getId() const;
	/**
	 * @brief Gives a pointer to the parent item of the current item.
	 *
	 * If the item has no parent (e.g. if it is a top element), the pointer
	 * is null.
	 *
	 * @return a pointer to the parent item of the current item
	 */
	GraphItem *getParent() const;
	/**
	 * @brief Gets one of the "Symbol", "Dir", or "File" value.
	 *
	 * @param i the field to get:
	 * <ul>
	 * <li>0 -> Symbol</li>
	 * <li>1 -> Dir</li>
	 * <li>2 -> File</li>
	 * </ul>
	 *
	 * @return the value of the field asked for or a default-constructed
	 * QVariant if i<0 or i>2.
	 */
	QVariant getValue(int i) const;

	/**
	 * @brief Sets the value of the "Symbol" field of the item.
	 *
	 * @param symbol the new symbol value
	 */
	void setSymbol(QString symbol);
	/**
	 * @brief Sets the value of the "Dir" field of the item.
	 *
	 * @param dir the new dir value
	 */
	void setDir(QString dir);
	/**
	 * @brief Sets the value of the "File" field of the item.
	 *
	 * @param file the new file value
	 */
	void setFile(QString file);
	/**
	 * @brief Sets the identifier of the item.
	 *
	 * @param id the new identifier
	 */
	void setId(quint64 id);
	/**
	 * @brief Sets the value of one of the fields of the item.
	 *
	 * @param i the field to set
	 * <ul>
	 * <li>0 -> Symbol</li>
	 * <li>1 -> Dir</li>
	 * <li>2 -> File</li>
	 * </ul>
	 * @param v the new value for the field
	 *
	 * @return true if, and only if, the value i was correct and one of the
	 * field could be set.
	 */
	bool setValue(int i, QVariant v);

	/**
	 * @brief Gives the number of children the item has.
	 *
	 * @return the number of children for this item
	 */
	int childrenCount() const;
	/**
	 * @brief Gives a pointer to the @p i -th child of the item.
	 *
	 * @param i the index of the child to retrieve, between 0 and
	 * childrenCount() - 1
	 *
	 * @return a pointer to the child if it exists, or null if the index
	 * @p i is invalid
	 */
	GraphItem* child(int i) const;
	/**
	 * @brief Gives the index of the current item in the list of its parent
	 * item's children
	 *
	 * @return the index of the current item among its siblings, or 0 if it
	 * has no parent
	 */
	int childNumber() const;

	/**
	 * @brief Add a child item to the item.
	 *
	 * @param i the index after which the child must be inserted, as with
	 * QList::insert()
	 * @param newChild the child to insert
	 */
	void addChild(int i, const GraphItem& newChild);
	/**
	 * @brief Removes a child from the item.
	 *
	 * @param i the index of the child to remove
	 *
	 * @return true if, and only if, the index is valid, i.e.@ there was
	 * a child with index @p i
	 */
	bool removeChild(int i);

	/**
	 * @brief the fields in the item
	 */
	static constexpr const char* const COLUMNS[] = { "Symbol", "Dir", "File" };
	/**
	 * @brief the number of fields in the item
	 *
	 * @return the number of fields in a GraphItem
	 */
	static constexpr int columnCount() { return 3; }

private:
	/**
	 * @brief the identifier of the item
	 */
	quint64 _id = 0;
	/**
	 * @brief the value for the "Symbol" field
	 */
	QString _symbol;
	/**
	 * @brief the value for the "Dir" field
	 */
	QString _dir;
	/**
	 * @brief the value for the "File" field
	 */
	QString _file;
	/**
	 * @brief the parent item of the item, if any
	 */
	GraphItem* _parent;
	/**
	 * @brief the children items of the item
	 */
	QList<GraphItem*> _children;
};

#endif // GRAPHITEM_H
