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
	bool isHighlighted();
	bool isUnhighlighted();

signals:

public slots:

protected:
	Graph* _graph;
	QAbstractGraphicsShapeItem* _inner;

	static const QPen DEFAULT_PEN;
	static const QPen HIGHLIGHT_PEN;
};

#endif // ELEMENT_H
