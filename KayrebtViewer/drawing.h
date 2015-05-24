#ifndef DRAWING_H
#define DRAWING_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "graph.h"

class Drawing : public QGraphicsView
{
	Q_OBJECT

public:
	explicit Drawing(QString filename, QWidget *parent = 0);
	~Drawing();

protected:
	virtual void wheelEvent(QWheelEvent *event);

private:
	QGraphicsScene *_graph;
};

#endif // DRAWING_H
