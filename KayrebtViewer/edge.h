/**
 * @file edge.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class Edge
 */
#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsObject>
#include <QGraphicsPathItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsPolygonItem>
#include <gvc.h>
#include <types.h>
#include "element.h"

class Graph;

/**
 * @brief This class represents an edge in a diagram.
 * @todo Use github://nbergont/qgv?
 */
class Edge : public Element
{
	Q_OBJECT
public:
	/**
	 * @brief Constructor.
	 *
	 * Builds the Edge from a GraphViz edge, in diagram \p graph.
	 *
	 * @param v the GraphViz structure representing an edge
	 * @param graph the diagram in which the edge is added
	 * @param parent the parent item, probebly the graph
	 */
	explicit Edge(Agedge_t *v, Graph* graph, QGraphicsItem *parent = 0);

	virtual ~Edge();
	/**
	 * @brief Mark the edge as invisible.
	 */
	virtual void hide();
	/**
	 * @brief Tells whether the Edge has an ancestor which is highlighted.
	 *
	 * @return true if and only if at least one ancestor is highlighted
	 */
	virtual bool hasHighlightedAncestor() const;
	virtual void highlight() override;
	virtual void unhighlight() override;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:

public slots:
	/**
	 * @brief The slot is triggered when the Edge receives a double-click.
	 *
	 * @param UNUSED <i>unused</i>
	 */
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * UNUSED);

private:
	/**
	 * @brief the corresponding GraphViz edge
	 */
	Agedge_t* _gv_edge;
	/**
	 * @brief the text of the edge (possibly null)
	 */
	QGraphicsSimpleTextItem* _label;
	/**
	 * @brief the head of the edge
	 */
	QGraphicsPolygonItem* _arrowHead;

friend class Graph;
};

#endif // EDGE_H
