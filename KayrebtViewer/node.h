/**
 * @file node.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class Node
 */
#ifndef NODE_H
#define NODE_H

#include <QGraphicsObject>
#include <QGraphicsSimpleTextItem>
#include <QAbstractGraphicsShapeItem>
#include <QPen>
#include <QFont>
#include <gvc.h>
#include <types.h>
#include "element.h"

class Graph;


/**
 * @brief This class represents a node in a diagram, associated with the
 * corresponding GraphViz data structure.
 */
class Node : public Element
{
	Q_OBJECT
public:
	/**
	 * @brief Constructor. Builds a node from a GraphViz Agnode_t.
	 *
	 * @param v the GraphViz data structure representing the node
	 * @param graph the graph in which the Node is added
	 * @param parent the parent object of the Node
	 */
	explicit Node(Agnode_t *v, Graph* graph);
	/**
	 * @brief Destroys the Node and frees associated GraphViz's resources.
	 */
	~Node();
	/**
	 * @brief Mark the Node as invisible.
	 *
	 * The Node continues occupying as much place as before and the graph
	 * is not laid out again.
	 */
	virtual void hide();

	virtual bool hasHighlightedAncestor() const override;

signals:
	/**
	 * @brief This signal is emitted when a user activates the hyperlink
	 * contained in the node.
	 *
	 * This signal can be emitted only if the Node embeds a URL attribute.
	 *
	 * @param url the URL emitted
	 */
	void hyperlinkActivated(const QString& url);

public slots:
	/**
	 * @brief Hides the node.
	 *
	 * This slot is triggered when the user double-clicks on the Node.
	 *
	 * @param UNUSED <i>unused</i>
	 */
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * UNUSED);
	/**
	 * @brief Highlights the Node and all the elements of the diagram
	 * reachable from it.
	 *
	 * This slot is triggered when the user hovers the node with its mouse
	 * pointer.
	 *
	 * @param UNUSED <i>unused</i>
	 */
	void hoverEnterEvent(QGraphicsSceneHoverEvent * UNUSED);
	/**
	 * @brief Reverses the effect of hoverEnterEvent().
	 *
	 * This slot is triggered when the mouse pointer leaves the Node.
	 *
	 * @param UNUSED <i>unused</i>
	 */
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * UNUSED);
	/**
	 * @brief Activates the hyperlink in the Node if the user holds key
	 * Ctrl.
	 *
	 * This slot is triggered when the user clicks on the Node.
	 *
	 * @param UNUSED <i>unused</i>
	 */
	void mousePressEvent(QGraphicsSceneMouseEvent* UNUSED);

signals:
	void nodeWithLineInformationHovered(int line, QString& filename);

private:
	/**
	 * @brief the GraphViz data structure representing the node
	 */
	Agnode_t* _gv_node;
	/**
	 * @brief the text of the Node
	 */
	QGraphicsSimpleTextItem* _label;
	/**
	 * @brief the URL carried by the node, possibly empty
	 */
	QString _url;

	int _line = 0;
	QString _file;

	/**
	 * @brief Draws the Node, using GraphViz's layout information
	 *
	 * @return the Node shape, without the text inside
	 */
	QAbstractGraphicsShapeItem* draw();

friend class Graph;
};

#endif // NODE_H
