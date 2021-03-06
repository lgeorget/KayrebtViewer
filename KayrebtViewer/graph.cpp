/**
 * @file graph.cpp
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Implementation of class Graph
 */
#include <cstdio>
#include <cstring>
#include <vector>
#include <memory>
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
#include <QMutex>
#include <QtCore>
#include <types.h>
#include "viewer.h"
#include "graph.h"
#include "edge.h"
#include "node.h"
#include "hyperlinkactivatedevent.h"
#include "nodehoverevent.h"

const qreal Graph::DOT_DEFAULT_DPI = 72.0;
const QFont Graph::MONOSPACE_FONT = QFont("Monospace", 15, QFont::Normal);

QMutex Graph::_graphviz;

namespace {
extern "C" {
	typedef struct
	{
		Agrec_t h;
		Node* node;
	} Kayrebtnodeinfo_t;

	typedef struct
	{
		Agrec_t h;
		Edge* edge;
	} Kayrebtedgeinfo_t;
}

	inline Node* getNode(Agnode_t* n) {
		Kayrebtnodeinfo_t* k = reinterpret_cast<Kayrebtnodeinfo_t*>(aggetrec(n,"kayrebt",false));
		return k ? k->node : nullptr;
	}

	inline void setNode(Agnode_t* n, Node* node) {
		Kayrebtnodeinfo_t* k = reinterpret_cast<Kayrebtnodeinfo_t*>(aggetrec(n,"kayrebt",false));
		if (k)
			k->node = node;
	}

	inline Edge* getEdge(Agedge_t* e) {
		Kayrebtedgeinfo_t* k = reinterpret_cast<Kayrebtedgeinfo_t*>(aggetrec(e,"kayrebt",false));
		return k ? k->edge : nullptr;
	}

	inline void setEdge(Agedge_t* e, Edge* edge) {
		Kayrebtedgeinfo_t* k = reinterpret_cast<Kayrebtedgeinfo_t*>(aggetrec(e,"kayrebt",false));
		if (k)
			k->edge = edge;
	}
}

Graph::Graph(quint64 id, const QString& filename, QObject* parent) : QGraphicsScene(parent), _id(id), _gv_con(Viewer::GRAPHVIZ_CONTEXT), _graph(), _filename(filename)
{
	std::FILE* fp = fopen(qPrintable(filename), "r");
	if (fp == nullptr)
		throw std::runtime_error("Couldn't open input graph");

	_graph = agread(fp, nullptr);
	if (!_graph)
		throw std::runtime_error("Couldn't parse graph from input file");

	aginit(_graph, AGNODE, "kayrebt", sizeof(Kayrebtnodeinfo_t), false);
	aginit(_graph, AGEDGE, "kayrebt", sizeof(Kayrebtedgeinfo_t), false);
	_dpi = QString(agget(_graph,"dpi")).toDouble(); // cannot access through GD_drawing(_graph)->_dpi as the layout isn't done yet
	if (_dpi == 0)
		_dpi = 96.0;

	_sourceFilename = QString(agget(_graph,"file"));
	_sourceLineNumber = QString(agget(_graph,"line")).toInt();

	setAttrs();
	connect(this, SIGNAL(layoutDone()), this, SLOT(doBuild()));
}

Graph::~Graph()
{
	_graphviz.lock();
	gvFreeLayout(_gv_con, _graph);
	_graphviz.unlock();
	agclose(_graph);
}

void Graph::build()
{
	QtConcurrent::run(this, &Graph::doLayout);
}

void Graph::doBuild()
{
	for (Agnode_t* v = agfstnode(_graph) ; v ; v = agnxtnode(_graph,v)) {
		addNode(v);
		for (Agedge_t* e = agfstout(_graph,v) ; e ; e = agnxtout(_graph,e))
			addEdge(e);
	}
	emit graphBuilt();
}

void Graph::pimpSubTree(Node *n, std::function<void (Element &)> f, std::function<bool (Element&)> test, bool incomingEdgesAreConcerned)
{
	Agnode_t* v = n->_gv_node;
	QQueue<Agnode_t*> waitingNodes;
	QList<Agnode_t*> finishedNodes;

	waitingNodes.enqueue(v);
	while (!waitingNodes.empty()) {
		Agnode_t* currentNode = waitingNodes.dequeue();
		f(*getNode(currentNode));
		if (incomingEdgesAreConcerned) {
			for (Agedge_t* in = agfstin(_graph,currentNode) ; in ; in = agnxtin(_graph,in)) {
				f(*getEdge(in));
			}
		}

		for (Agedge_t* e = agfstout(_graph,currentNode) ; e ; e = agnxtout(_graph,e)) {
			Agnode_t* nextNode = aghead(e);
			bool toProcess = true;
			if (test != nullptr) {
				for (Agedge_t* in = agfstin(_graph,nextNode) ; in && toProcess ; in = agnxtin(_graph,in)) {
					Node* tested = getNode(agtail(in));
					if (test(*tested))
						toProcess = false;
				}
			}
			f(*getEdge(e));
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
	Node *n = getNode(aghead(e->_gv_edge));
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

const QString& Graph::getSourceFilename() const
{
	return _sourceFilename;
}

int Graph::getSourceLine() const
{
	return _sourceLineNumber;
}

void Graph::setAttrs()
{
	agattr(_graph, AGNODE, "fontname", const_cast<char*>(qPrintable(MONOSPACE_FONT.family())));
	agattr(_graph, AGNODE, "fontsize", const_cast<char*>(qPrintable(QString("%1").arg(MONOSPACE_FONT.pointSizeF()*DOT_DEFAULT_DPI/_dpi))));
}

void Graph::doLayout()
{
	_graphviz.lock();
	gvFreeLayout(_gv_con, _graph);
	gvLayout(_gv_con, _graph, "dot");
	_graphviz.unlock();
	setSceneRect(GD_bb(_graph).LL.x*(_dpi/DOT_DEFAULT_DPI), GD_bb(_graph).LL.y*(_dpi/DOT_DEFAULT_DPI),
				 GD_bb(_graph).UR.x*(_dpi/DOT_DEFAULT_DPI), GD_bb(_graph).UR.y*(_dpi/DOT_DEFAULT_DPI));
	emit layoutDone();
}

QString Graph::getFilename() const
{
	return _filename;
}

void Graph::addNode(Agnode_t* v)
{
	_nodes.emplace_back(new Node(v,this));
	Node* node = _nodes.back().get();
	addItem(node);
	setNode(v,node);
}

void Graph::addEdge(Agedge_t* e)
{
	_edges.emplace_back(new Edge(e,this));
	Edge* edge = _edges.back().get();
	addItem(edge);
	setEdge(e,edge);
}

bool Graph::hasHighlightedAncestor(const Node* n)
{
	bool ancestorHighlighted = false;
	for (Agedge_t* in = agfstin(_graph,n->_gv_node) ; in && !ancestorHighlighted ; in = agnxtin(_graph,in)) {
		ancestorHighlighted = getEdge(in)->isHighlighted();
	}
	return ancestorHighlighted;
}

bool Graph::hasHighlightedAncestor(const Edge* e)
{
	return hasHighlightedAncestor(getNode(agtail(e->_gv_edge)));
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

void Graph::highlightLineInSourceCode(int line, QString& file)
{
	NodeHoverEvent hovering(line,file);
	QList<QWidget*> topLevels = qApp->topLevelWidgets();
	for (int i=0 ; i<topLevels.size() ; i++)
		if (topLevels[i]->isActiveWindow()) // send the event to the main window
			qApp->sendEvent(topLevels[i], &hovering);
}

void Graph::reset()
{
	for (Agnode_t* v = agfstnode(_graph) ; v ; v = agnxtnode(_graph,v)) {
		agsafeset(v, "style", "normal", "normal");
		getNode(v)->setVisible(true);
		getNode(v)->unhighlight();
		for (Agedge_t* e = agfstout(_graph,v) ; e ; e = agnxtout(_graph,e)) {
			agsafeset(e, "style", "normal", "normal");
			getEdge(e)->setVisible(true);
			getEdge(e)->unhighlight();
		}
	}
}

qint64 Graph::getId() const
{
	return _id;
}
