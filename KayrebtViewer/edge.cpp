#include <QFont>
#include <QDebug>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsPathItem>
#include <QGraphicsPolygonItem>
#include <qmath.h>
#include "edge.h"
#include "graph.h"

Edge::Edge(Agedge_t *e, Graph *graph, QGraphicsItem *parent) :
	Element(graph, parent),
	_gv_edge(e)
{
	qreal dpi = _graph->getDpi();
	qreal scale = dpi/_graph->DOT_DEFAULT_DPI;
	const Agraph_t* g = _graph->getAgraph();

	QPainterPath path;

	// -----BEGIN SHAMELESSLY COPY-PASTED CODE-----
	// © Steve D. Lazaro, 07/2010
	// Taken from http://www.mupuf.org/blog/2010/07/08/how_to_use_graphviz_to_draw_graphs_in_a_qt_graphics_scene/
	//
	//Calculate the path from the spline (only one spline, as the graph is strict). If it
	//waren't, we would have to iterate over the first list too.
	if ((ED_spl(e)->list!=0) && (ED_spl(e)->list->size%3 == 1))
	{
		//If there is a starting point, draw a line from it to the first curve point
		if(ED_spl(e)->list->sflag)
		{
			path.moveTo(ED_spl(e)->list->sp.x*scale,
						 (GD_bb(g).UR.y - ED_spl(e)->list->sp.y)*scale);
			path.lineTo(ED_spl(e)->list->list[0].x*scale,
					(GD_bb(g).UR.y - ED_spl(e)->list->list[0].y)*scale);
		}
		else
			path.moveTo(ED_spl(e)->list->list[0].x*scale,
					(GD_bb(g).UR.y - ED_spl(e)->list->list[0].y)*scale);

		//Loop over the curve points
		for(int i=1; i<ED_spl(e)->list->size; i+=3)
			path.cubicTo(ED_spl(e)->list->list[i].x*scale,
				  (GD_bb(g).UR.y - ED_spl(e)->list->list[i].y)*scale,
				  ED_spl(e)->list->list[i+1].x*scale,
				  (GD_bb(g).UR.y - ED_spl(e)->list->list[i+1].y)*scale,
				  ED_spl(e)->list->list[i+2].x*scale,
				  (GD_bb(g).UR.y - ED_spl(e)->list->list[i+2].y)*scale);

		//If there is an ending point, draw a line to it
		if(ED_spl(e)->list->eflag)
			path.lineTo(ED_spl(e)->list->ep.x*scale,
						 (GD_bb(g).UR.y - ED_spl(e)->list->ep.y)*scale);


		// draw the arrow
		QPointF cur(path.currentPosition());
		qreal angleRad = (180.0f - path.angleAtPercent(1)) * 3.14159265358979 / 180.0f;

		//debug
		//QGraphicsSimpleTextItem* angleLabel = new QGraphicsSimpleTextItem(QString("%1").arg(angleRad),this);
		//angleLabel->setPos(cur + QPointF(5, -20));

		QVector<QPointF> pp;
		pp << cur
		   << cur + QPointF(15 * qCos(angleRad-0.3), 15 * qSin(angleRad-0.3))
		   << cur + QPointF(15 * qCos(angleRad+0.3), 15 * qSin(angleRad+0.3))
		   << cur;
		path.addPolygon(QPolygonF(pp));
	}
	// -----END SHAMELESSLY COPY-PASTED CODE-----
	if (ED_label(e)) {
		_label = new QGraphicsSimpleTextItem(ED_label(e)->text, this);
		QPointF labelPos(ED_label(e)->pos.x*scale - ED_label(e)->dimen.x*scale/2, (GD_bb(g).UR.y - ED_label(e)->pos.y)*scale);
		_label->setPos(labelPos);
		_label->setFont(Graph::MONOSPACE_FONT);
	}

	_inner = new QGraphicsPathItem(path,this);
	_inner->setPen(defaultPen);
}

void Edge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
	_graph->pimpSubTree(this,&Element::hide,&Edge::isVisible);
}

void Edge::hide()
{
	agsafeset(_gv_edge,"style","invisible","invisible");
	QGraphicsItem::hide();
}

bool Edge::hasHighlightedAncestor() const
{
	return _graph->hasHighlightedAncestor(this);
}
