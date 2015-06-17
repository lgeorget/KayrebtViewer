/**
 * @file element.cpp
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Implementation of class Element
 */
#include <QPen>
#include <QBrush>
#include <QRectF>
#include "graph.h"
#include "element.h"

QPen Element::defaultPen = QPen(QBrush(QColor("mediumaquamarine")), 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
QBrush Element::defaultBrush = QBrush(QColor("mediumaquamarine"));
QPen Element::highlightPen = QPen(QBrush(QColor("red")), 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);

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
	_inner->setPen(highlightPen);
}

void Element::unhighlight()
{
	_inner->setPen(defaultPen);
}

bool Element::isHighlighted() const
{
	return _inner->pen() == highlightPen;
}

bool Element::isUnhighlighted() const
{
	return _inner->pen() == defaultPen;
}

