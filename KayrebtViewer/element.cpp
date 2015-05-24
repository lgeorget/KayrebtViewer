#include <QPen>
#include <QBrush>
#include <QRectF>
#include "graph.h"
#include "element.h"

const QPen Element::DEFAULT_PEN = QPen(QBrush(QColor("mediumaquamarine")), 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
const QPen Element::HIGHLIGHT_PEN = QPen(QBrush(QColor("red")), 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);

Element::Element(Graph *graph, QGraphicsItem *parent) :
	QGraphicsObject(parent),
	_graph(graph)
{
}

Element::~Element()
{
	delete _inner;
}

QRectF Element::boundingRect() const
{
	return _inner->boundingRect();
}

void Element::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	_inner->paint(painter, option, widget);
}

void Element::highlight()
{
	_inner->setPen(HIGHLIGHT_PEN);
}

void Element::unhighlight()
{
	_inner->setPen(DEFAULT_PEN);
}

bool Element::isHighlighted()
{
	return _inner->pen() == HIGHLIGHT_PEN;
}

bool Element::isUnhighlighted()
{
	return _inner->pen() == DEFAULT_PEN;
}
