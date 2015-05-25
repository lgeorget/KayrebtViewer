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

class Node : public Element
{
	Q_OBJECT
public:
	explicit Node(Agnode_t *v, Graph* graph, QGraphicsItem *parent = 0);
	~Node();
	virtual void hide();
	bool hasHighlightedAncestor() const;

signals:

public slots:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *);

private:
	Agnode_t* _gv_node;
	QGraphicsSimpleTextItem* _label;

	QAbstractGraphicsShapeItem* draw();

friend class Graph;
};

#endif // NODE_H
