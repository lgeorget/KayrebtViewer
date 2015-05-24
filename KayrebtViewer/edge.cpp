#include <QFont>
#include <QDebug>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsPathItem>
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
	//Calculate the path from the spline (only one spline, as the graph is strict. If it
	//wasn't, we would have to iterate over the first list too)
	//Calculate the path from the spline (only one as the graph is strict)
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
	}
	// -----END SHAMELESSLY COPY-PASTED CODE-----

	_inner = new QGraphicsPathItem(path,this);
	_inner->setPen(DEFAULT_PEN);
}

void Edge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	_graph->pimpSubTree(this,&Element::hide,&Edge::isVisible);
}

void Edge::hide()
{
	agsafeset(_gv_edge,"style","invisible","invisible");
	QGraphicsItem::hide();
}
