#ifndef DRAWING_H
#define DRAWING_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

class Graph;

class Drawing : public QGraphicsView
{
	Q_OBJECT

public:
	explicit Drawing(QString filename, QWidget *parent = 0);
	~Drawing();

public slots:
	void showContextMenu(const QPoint& point);

protected:
	virtual void wheelEvent(QWheelEvent *event);

private:
	Graph *_graph;
};

#endif // DRAWING_H
