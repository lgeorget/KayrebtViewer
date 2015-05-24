#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsObject>
#include <QGraphicsPathItem>
#include <gvc.h>
#include <types.h>
#include "element.h"

class Graph;

class Edge : public Element
{
	Q_OBJECT
public:
	explicit Edge(Agedge_t *v, Graph* graph, QGraphicsItem *parent = 0);
	virtual void hide();

signals:

public slots:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
	Agedge_t* _gv_edge;

friend class Graph;
};

#endif // EDGE_H
