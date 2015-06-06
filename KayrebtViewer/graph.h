#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QGraphicsScene>
#include <gvc.h>
#include <QGraphicsScene>
#include <QMap>
#include <QPair>
#include <QFont>
#include <functional>

class Node;
class Edge;
class Element;

class Graph : public QGraphicsScene
{
	Q_OBJECT

public:
	Graph(quint64 id, const QString& filename, QObject* parent = nullptr);
	~Graph();
	void pimpSubTree(Node *n, std::function<void (Element &)> f, std::function<bool (Element&)> test = nullptr, bool incomingEdgesAreConcerned =  false);
	void pimpSubTree(Edge *e, std::function<void (Element &)> f, std::function<bool (Element&)> test = nullptr);

	static const qreal DOT_DEFAULT_DPI;
	static const QFont MONOSPACE_FONT;

	const Agraph_t* getAgraph() const;
	qreal getDpi() const;
	bool hasHighlightedAncestor(const Node* n);
	bool hasHighlightedAncestor(const Edge* e);
	void callOtherGraph(QString url);

	QString getFilename() const;

public slots:
	void doLayout();
	void reset();

private:
	void addNode(Agnode_t* v);
	void addEdge(Agedge_t* v);
	void setAttrs();
	quint64 _id = 0;
	GVC_t* _gv_con;
	Agraph_t* _graph;
	qreal _dpi;
	QMap<int,Node*> _nodes;
	QMap<int,Edge*> _edges;
	QString _filename;

};

#endif // GRAPH_H
