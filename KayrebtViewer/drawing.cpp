#include <QProcess>
#include <QDebug>
#include <QtAlgorithms>
#include <exception>
#include <QWheelEvent>
#include <QInputEvent>
#include <QEvent>
#include <QMenu>
#include "graph.h"
#include "drawing.h"


Drawing::Drawing(QString inputFileName, QWidget *parent) :
	QGraphicsView(parent)
{
	_graph = new Graph(inputFileName, this);
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setScene(_graph);
	setDragMode(QGraphicsView::ScrollHandDrag);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

void Drawing::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers() == Qt::ControlModifier) {
		setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	} else {
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	}

	qreal scaleFactor = 1.2;
	if (event->delta() > 0)
		this->scale(scaleFactor, scaleFactor);
	else
		this->scale(1.0/scaleFactor, 1.0/scaleFactor);
}

Drawing::~Drawing()
{
	delete _graph;
}

void Drawing::showContextMenu(const QPoint &point)
{
	QPoint globalPos = mapToGlobal(point);
	QMenu contextualMenu;

	QAction* resetAction = contextualMenu.addAction(tr("reset"));
	QAction* act = contextualMenu.exec(globalPos);
	if (act) {
		if (act == resetAction)
			_graph->reset();
	}
}
