/**
 * @file drawing.cpp
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Implementation of class Drawing
 */
#include <QProcess>
#include <QDebug>
#include <exception>
#include <QWheelEvent>
#include <QInputEvent>
#include <QEvent>
#include <QMenu>
#include <QTimer>
#include <QtCore>
#include "graph.h"
#include "drawing.h"

Drawing::Drawing(quint64 id, QString inputFileName, QWidget *parent) :
	QGraphicsView(parent)
{
	_graph = new Graph(id, inputFileName, this);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

	QGraphicsScene* temporaryScene = new QGraphicsScene();
	temporaryScene->addText("Graph in construction, a moment please...\n (remember I can draw only one graph at a time (wait until next release ;) )");
	temporaryScene->setBackgroundBrush(QBrush(Qt::gray));
	setScene(temporaryScene);

	connect(_graph, SIGNAL(graphBuilt()), this, SLOT(setGraphReady()));
	_graph->build();
}

void Drawing::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers() == Qt::ControlModifier) {
		setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	} else {
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	}

	const qreal scaleFactor = 1.2;
	if (event->delta() > 0)
		this->scale(scaleFactor, scaleFactor);
	else
		this->scale(1.0/scaleFactor, 1.0/scaleFactor);
}

Drawing::~Drawing()
{
	_graph->deleteLater();
}

void Drawing::zoomToFit()
{
	fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void Drawing::setGraphReady()
{
	_graphReady = true;
	QGraphicsScene* oldScene = scene();
	setScene(_graph);
	oldScene->deleteLater();

	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setDragMode(QGraphicsView::ScrollHandDrag);
	invalidateScene();
	disconnect(_graph, SIGNAL(graphBuilt()), this, SLOT(setGraphReady()));
}

void Drawing::paintEvent(QPaintEvent *event)
{
	if (!_alreadyShown && _graphReady) {
		_alreadyShown = true;
		zoomToFit();
	}

	QGraphicsView::paintEvent(event);
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

qint64 Drawing::getId() const
{
	return _graph->getId();
}
