#include <cstdio>
#include <cstring>
#include <QString>
#include <exception>
#include <QGraphicsPathItem>
#include <QApplication>
#include <QGraphicsView>
#include <QPair>
#include <QQueue>
#include <QDebug>
#include <QEvent>
#include <QFileInfo>
#include <QSettings>
#include <types.h>
#include "graph.h"
#include "edge.h"
#include "node.h"
#include "hyperlinkactivatedevent.h"

const qreal Graph::DOT_DEFAULT_DPI = 72.0;
const QFont Graph::MONOSPACE_FONT = QFont("Monospace", 15, QFont::Normal);

Graph::Graph(quint64 id, const QString& filename, QObject *parent) : QGraphicsScene(parent), _id(id), _gv_con(gvContext()), _graph(), _filename(filename)
{
	std::FILE* fp = fopen(qPrintable(filename), "r");
	if (fp == nullptr)
		throw std::runtime_error("Couldn't open input graph");

	_graph = agread(fp);
	if (!_graph)
		throw std::runtime_error("Couldn't parse graph from input file");

	_dpi = QString(agget(_graph,"dpi")).toDouble(); // cannot access through GD_drawing(_graph)->_dpi as the layout isn't done yet
	if (_dpi == 0)
		_dpi = 96.0;

	setAttrs();
	doLayout();

	for (Agnode_t* v = agfstnode(_graph) ; v ; v = agnxtnode(_graph,v)) {
		addNode(v);
		for (Agedge_t* e = agfstout(_graph,v) ; e ; e = agnxtout(_graph,e))
			addEdge(e);
	}
}

Graph::~Graph()
{
	gvFreeLayout(_gv_con, _graph);
	gvFreeContext(_gv_con);
	qDeleteAll(_nodes);
	qDeleteAll(_edges);
}

void Graph::pimpSubTree(Node *n, std::function<void (Element &)> f, std::function<bool (Element&)> test, bool incomingEdgesAreConcerned)
{
	Agnode_t* v = n->_gv_node;
	QQueue<Agnode_t*> waitingNodes;
	QList<Agnode_t*> finishedNodes;

	waitingNodes.enqueue(v);
	while (!waitingNodes.empty()) {
		Agnode_t* currentNode = waitingNodes.dequeue();
		f(*_nodes[currentNode->id]);
		if (incomingEdgesAreConcerned) {
			for (Agedge_t* in = agfstin(_graph,currentNode) ; in ; in = agnxtin(_graph,in)) {
				f(*_edges[in->id]);
			}
		}

		for (Agedge_t* e = agfstout(_graph,currentNode) ; e ; e = agnxtout(_graph,e)) {
			Agnode_t* nextNode = e->head;
			bool toProcess = true;
			if (test != nullptr) {
				for (Agedge_t* in = agfstin(_graph,nextNode) ; in && toProcess ; in = agnxtin(_graph,in)) {
					Node* tested = _nodes[in->tail->id];
					if (test(*tested))
						toProcess = false;
				}
			}
			f(*_edges[e->id]);
			if (toProcess && !waitingNodes.contains(nextNode) && !finishedNodes.contains(nextNode)) {
				waitingNodes.enqueue(nextNode);
			}
		}
		finishedNodes.append(currentNode);
	}
}

void Graph::pimpSubTree(Edge *e, std::function<void (Element &)> f, std::function<bool (Element&)> test)
{
	f(*e);
	Node *n = _nodes[e->_gv_edge->head->id];
	if (!test(*n))
		pimpSubTree(n,f,test);
}

const Agraph_t *Graph::getAgraph() const
{
	return _graph;
}

qreal Graph::getDpi() const
{
	return _dpi;
}

void Graph::setAttrs()
{
	agnodeattr(_graph, "fontname", const_cast<char*>(qPrintable(MONOSPACE_FONT.family())));
	agnodeattr(_graph, "fontsize", const_cast<char*>(qPrintable(QString("%1").arg(MONOSPACE_FONT.pointSizeF()*DOT_DEFAULT_DPI/_dpi))));
}

void Graph::doLayout()
{
	gvFreeLayout(_gv_con, _graph);
	gvLayout(_gv_con, _graph, "dot");

	setSceneRect(GD_bb(_graph).LL.x*(_dpi/DOT_DEFAULT_DPI), GD_bb(_graph).LL.y*(_dpi/DOT_DEFAULT_DPI),
				  GD_bb(_graph).UR.x*(_dpi/DOT_DEFAULT_DPI), GD_bb(_graph).UR.y*(_dpi/DOT_DEFAULT_DPI));
}

QString Graph::getFilename() const
{
	return _filename;
}

void Graph::addNode(Agnode_t* v)
{
	Node* node = new Node(v,this);
	addItem(node);
	_nodes[v->id] = node;
}

void Graph::addEdge(Agedge_t* e)
{
	Edge* edge = new Edge(e,this);
	addItem(edge);
	_edges[e->id] = edge;
}

bool Graph::hasHighlightedAncestor(const Node* n)
{
	bool ancestorHighlighted = false;
	for (Agedge_t* in = agfstin(_graph,n->_gv_node) ; in && !ancestorHighlighted ; in = agnxtin(_graph,in)) {
		ancestorHighlighted = _edges[in->id]->isHighlighted();
	}
	return ancestorHighlighted;
}

bool Graph::hasHighlightedAncestor(const Edge* e)
{
	return hasHighlightedAncestor(_nodes[e->_gv_edge->tail->id]);
}

void Graph::callOtherGraph(QString url)
{
	//Here, we have to tweak the URL
	// Two cases: 1) the URL references a local (static) function, in this case, it will not have
	//               a complete path
	//            2) the URL references a global function, in this case, it will have a path relative
	//               to the source tree

	if (url.startsWith("./") && url.count("/") == 1) {
		// Case 1)
		url = QFileInfo(_filename).absolutePath() +	"/" + url + ".dot";
	} else {
		// Case 2)
		QSettings settings;
		url = settings.value("diagrams dir").toString() + url + ".dot";
	}

	HyperlinkActivatedEvent hyperlink(_id, url);
	//qDebug() << "new Hyperlink event " << &hyperlink;
	QList<QWidget*> topLevels = qApp->topLevelWidgets();
	for (int i=0 ; i<topLevels.size() ; i++)
		if (topLevels[i]->isActiveWindow()) // send the event to the main window
			qApp->sendEvent(topLevels[i], &hyperlink);
}

void Graph::reset()
{
	for (Agnode_t* v = agfstnode(_graph) ; v ; v = agnxtnode(_graph,v)) {
		agsafeset(v, "style", "normal", "normal");
		_nodes[v->id]->setVisible(true);
		_nodes[v->id]->unhighlight();
		for (Agedge_t* e = agfstout(_graph,v) ; e ; e = agnxtout(_graph,e)) {
			agsafeset(e, "style", "normal", "normal");
			_edges[e->id]->setVisible(true);
			_edges[e->id]->unhighlight();
		}
	}
}

