/**
 * @file graph.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class Graph
 */
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

/**
 * \brief This class is responsible for displaying a diagram from a file in dot
 * format.
 *
 * A Graph is a graphics scene populated with the element of the diagram.
 * The diagram is parsed from a file using the GraphViz library. The layout of
 * the nodes and edges is computed by GraphViz's dot and the result is displayed
 * by this class.
 */
class Graph : public QGraphicsScene
{
	Q_OBJECT

public:
	/**
	 * \brief Constructor. Loads a GraphViz file and builds the diagram
	 * from there.
	 *
	 * The diagram is entirely built in the constructor.
	 *
	 * \throw std::runtime_exception if the file does not exist or is not
	 * a GraphViz file
	 *
	 * \param id the identifier of the diagram
	 * \param filename the file where the diagram is stored
	 * \param parent the parent object of the diagram
	 */
	Graph(quint64 id, const QString& filename, QObject* parent = nullptr);
	/**
	 * \brief Destroys the diagram and frees associated GraphViz resources.
	 */
	~Graph();
	/**
	 * \brief Conditionally applies a function to all nodes and edges
	 * accessible from node \p n.
	 *
	 * The name subTree can be misleading because it is not required that
	 * the entire diagram be a tree or even an acyclic graph. Still, this
	 * function may give better results on acyclic graphs, depending on \p
	 * f.
	 *
	 * \param n the node from which the transformation is applied
	 * \param f the function to apply to each nodes and edges accessible
	 * from n
	 * \param test a predicate to decide whether to apply the function on
	 * an eligible node or edge
	 * \param incomingEdgesAreConcerned whether the transformation should
	 * also be applied on edge coming to \p n
	 */
	void pimpSubTree(Node *n, std::function<void (Element &)> f, std::function<bool (Element&)> test = nullptr, bool incomingEdgesAreConcerned =  false);
	/**
	 * \brief Conditionally applies a function to all nodes and edges
	 * accessible from edge \p e.
	 *
	 * The name subTree can be misleading because it is not required that
	 * the entire diagram be a tree or even an acyclic graph. Still, this
	 * function may give better results on acyclic graphs, depending on \p
	 * f.
	 *
	 * \param e the edge from which the transformation is applied
	 * \param f the function to apply to each nodes and edges accessible
	 * from n
	 * \param test a predicate to decide whether to apply the function on
	 * an eligible node or edge
	 */
	void pimpSubTree(Edge *e, std::function<void (Element &)> f, std::function<bool (Element&)> test = nullptr);

	/**
	 * \brief the dots-per-inch value used by dot in its layout information
	 */
	static const qreal DOT_DEFAULT_DPI;
	/**
	 * \brief a monospace font to use for all labels in the diagram
	 */
	static const QFont MONOSPACE_FONT;

	/**
	 * \brief Gets a pointer to the GraphViz structure representing the
	 * diagram.
	 *
	 * \return a pointer to the GraphViz structure representing the
	 * diagram
	 */
	const Agraph_t* getAgraph() const;
	/**
	 * \brief Gets the DPI resolution used for the graph
	 *
	 * \return the DPI value
	 */
	qreal getDpi() const;
	/**
	 * \brief Tells whether \p n has highlighted ancestor
	 *
	 * \param n the node of interest
	 *
	 * \return true if, and only if, the node \p n has at least one highlighted
	 * ancestor
	 */
	bool hasHighlightedAncestor(const Node* n);
	/**
	 * \brief Tells whether \p e has highlighted ancestor
	 *
	 * \param e the edge of interest
	 *
	 * \return true if, and only if, the edge \p e has at least one highlighted
	 * ancestor
	 */
	bool hasHighlightedAncestor(const Edge* e);
	/**
	 * \brief Sends an event to the main window to announce that a new child
	 * Graph of the current Graph must be open
	 *
	 * \param url the url of the other Graph, (usually it is embedded in a
	 * URL attribute in an element of the Graph)
	 */
	void callOtherGraph(QString url);

	/**
	 * \brief Gives the name of the file from which the diagram is extracted.
	 *
	 * \return the diagram filename
	 */
	QString getFilename() const;

public slots:
	/**
	 * \brief When this slots is triggered, the Graph asks GraphViz to do
	 * the layout of the diagram.
	 */
	void doLayout();
	/**
	 * \brief Restores the diagram to its initial state, with all the
	 * elements visible.
	 */
	void reset();

private:
	/**
	 * \brief Adds a node to the Graph.
	 *
	 * \param v the GraphViz node to add
	 */
	void addNode(Agnode_t* v);
	/**
	 * \brief Adds an edge to the Graph.
	 *
	 * \param v the GraphViz edge to add
	 */
	void addEdge(Agedge_t* v);
	/**
	 * \brief Sets some of the diagram's attributes to default values.
	 *
	 * This must be done before any laying out takes place.
	 */
	void setAttrs();
	/**
	 * \brief the diagram identifier
	 */
	quint64 _id = 0;
	/**
	 * \brief the context for GraphViz
	 */
	GVC_t* _gv_con;
	/**
	 * \brief the GraphViz graph
	 */
	Agraph_t* _graph;
	/**
	 * \brief the resolution DPI asked for
	 */
	qreal _dpi;
	/**
	 * \brief the nodes of the diagram, indexed by the GraphViz nodes' ids
	 *
	 * This structure can be used to get a pointer to a Node from the
	 * corresponding Agnode_t pointer.
	 * Example:
	 *
	 *     Node* getNodeFromAgnode_t(Agnode_t* n) {
	 *         return _nodes[n->id];
	 *     }
	 */
	QMap<int,Node*> _nodes;
	/**
	 * \brief the edges of the diagram, indexed by the GraphViz edges' ids
	 *
	 * This structure can be used to get a pointer to a Edge from the
	 * corresponding Agedge_t pointer.
	 * Example:
	 *
	 *     Node* getEdgeFromAgedge_t(Agedge_t* e) {
	 *         return _edges[e->id];
	 *     }
	 */
	QMap<int,Edge*> _edges;
	/**
	 * \brief the file (in GraphViz format) from which the diagram is read
	 */
	QString _filename;

};

#endif // GRAPH_H
