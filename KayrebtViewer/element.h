#ifndef ELEMENT_H
#define ELEMENT_H

#include <QGraphicsObject>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsItem>

class Graph;

class Element : public QGraphicsObject
{
	Q_OBJECT
public:
	explicit Element(Graph* graph, QGraphicsItem *parent = 0);
	virtual ~Element();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual QRectF boundingRect() const;
	void highlight();
	void unhighlight();
	bool isHighlighted() const;
	bool isUnhighlighted() const;
	virtual bool hasHighlightedAncestor() const = 0;

signals:

public slots:

protected:
	Graph* _graph;
	QAbstractGraphicsShapeItem* _inner;

	static QPen defaultPen;
	static QPen highlightPen;
	static QBrush defaultBrush;
};

#endif // ELEMENT_H
