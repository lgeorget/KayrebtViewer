#include <functional>
#include <QFont>
#include <QDebug>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <cstring>
#include "node.h"
#include "graph.h"
#include "element.h"

Node::Node(Agnode_t *v, Graph *graph, QGraphicsItem *parent) :
	Element(graph, parent),
	_gv_node(v),
	_label(new QGraphicsSimpleTextItem(this))
{
//	qDebug() << "dpi : " << dpi << " " << scale;

	_inner = draw();
	_inner->setPen(defaultPen);
	//qDebug() << "Adding node " << v->name << " at coords " << boundingRect() << " shape: " << agget(v,"shape");

	setFlags(QGraphicsItem::ItemIsSelectable);
	setAcceptHoverEvents(true);
}

QAbstractGraphicsShapeItem *Node::draw()
{
	qreal dpi = _graph->getDpi();
	qreal scale = dpi/_graph->DOT_DEFAULT_DPI;
	const Agraph_t* g = _graph->getAgraph();

	QString shape = agget(_gv_node, "shape");
	QAbstractGraphicsShapeItem* res;
	if (shape == "rect") {
		res = new QGraphicsRectItem(ND_coord(_gv_node).x*scale - ND_width(_gv_node)*dpi/2,
										 (GD_bb(g).UR.y - ND_coord(_gv_node).y)*scale - ND_height(_gv_node)*dpi/2,
										 ND_width(_gv_node)*dpi,
										 ND_height(_gv_node)*dpi,
										 this);
	} else if (shape == "diamond") {
		QPainterPath path;
		QPointF center(ND_coord(_gv_node).x*scale, (GD_bb(g).UR.y - ND_coord(_gv_node).y)*scale);
		QPointF top(center - QPointF(0, ND_height(_gv_node)*dpi/2));
		QPointF left(center - QPointF(ND_width(_gv_node)*dpi/2, 0));
		QPointF bottom(center + QPointF(0, ND_height(_gv_node)*dpi/2));
		QPointF right(center + QPointF(ND_width(_gv_node)*dpi/2, 0));

		path.moveTo(top);
		path.lineTo(left);
		path.lineTo(bottom);
		path.lineTo(right);
		path.lineTo(top);
		res = new QGraphicsPathItem(path, this);
	} else {
		res = new QGraphicsEllipseItem(ND_coord(_gv_node).x*scale - ND_width(_gv_node)*dpi/2,
										 (GD_bb(g).UR.y - ND_coord(_gv_node).y)*scale - ND_height(_gv_node)*dpi/2,
										 ND_width(_gv_node)*dpi,
										 ND_height(_gv_node)*dpi,
										 this);
	}
	QString label(agget(_gv_node,"label"));
	label.replace("\\n","\n");
	_label->setText(label);
	_label->setFont(_graph->MONOSPACE_FONT);
	QRectF rectLabel(_label->boundingRect());
	QRectF rectShape(res->boundingRect());
	qreal offset_w =  (rectShape.width() - rectLabel.width()) / 2;
	qreal offset_h =  (rectShape.height() - rectLabel.height()) / 2;
	_label->setPos(ND_coord(_gv_node).x*scale - ND_width(_gv_node)*dpi/2 + offset_w,
				  (GD_bb(g).UR.y - ND_coord(_gv_node).y)*scale - ND_height(_gv_node)*dpi/2 + offset_h);
	return res;
}

Node::~Node()
{
	delete _label;
}

void Node::hide()
{
	agsafeset(_gv_node,"style","invisible","invisible");
	QGraphicsItem::hide();
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)
{
	_graph->pimpSubTree(this, &Element::hide, &Node::isVisible,true);
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
	_graph->pimpSubTree(this,&Element::highlight);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	if (!isSelected() && !hasHighlightedAncestor()) {
		if (_graph->selectedItems() != QList<QGraphicsItem*>())
			_graph->pimpSubTree(this, &Element::unhighlight, &Element::hasHighlightedAncestor);
		else // no active selection
			_graph->pimpSubTree(this, &Element::unhighlight);
	}
}

bool Node::hasHighlightedAncestor() const
{
	return _graph->hasHighlightedAncestor(this);
}
