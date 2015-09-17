/**
 * @file element.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class Element
 */
#ifndef ELEMENT_H
#define ELEMENT_H

#include <QGraphicsObject>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsItem>

class Graph;

/**
 * @brief Parent class of objects belonging to diagrams.
 */
class Element : public QGraphicsObject
{
	Q_OBJECT
public:
	/**
	 * @brief Unique constructor.
	 *
	 * Builds an Element and mark it as owned by diagram @p graph.
	 *
	 * @param graph the diagram in which the Element exists
	 * @param parent the parent item of the Element, the diagram in most
	 * cases
	 */
	explicit Element(Graph* graph, QGraphicsItem *parent = nullptr);
	/**
	 * @brief Destroys the Element, and frees the associated GraphViz
	 * resources.
	 */
	virtual ~Element();
	/**
	 * @brief Displays the Element in the drawing area. Reimplementation
	 * from QGraphicsObject.
	 */
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	/**
	 * @brief Reimplemented from QGraphicsObject
	 *
	 * The default implementation of boundingRect() in the present class
	 * might need to be rewritten, if the item bounds are bigger than
	 * those of \a _inner.
	 *
	 * @return _inner->boundingRect()
	 */
	virtual QRectF boundingRect() const;
	/**
	 * @brief Highlights the Element, displaying it in a more eye-catching
	 * fashion.
	 *
	 * When it is highlighted, an Element is drawn using the \a
	 * Element::highlightPen.
	 */
	virtual void highlight();
	/**
	 * @brief Unhighlight the Element, displaying it as usual.
	 *
	 * When it is not highlighted, an Element is displayed using the \a
	 * Element::unhighlightPen.
	 */
	virtual void unhighlight();
	/**
	 * @brief Tells whether the Element is highlighted.
	 *
	 * @return true if, and only if, the Element is drawn with the \a
	 * Element::hilightPen.
	 */
	bool isHighlighted() const;
	/**
	 * @brief Tells if the Element is not highlighted.
	 *
	 * @return true, if, and only if, the Element is drawn with the
	 * \a Element::defaultPen.
	 */
	bool isUnhighlighted() const;
	/**
	 * @brief Tells is the Element has an highlighted ancestor.
	 *
	 * @return true if, and only if, the Element has at least one ancestor
	 * (in the diagram) highlighted
	 */
	virtual bool hasHighlightedAncestor() const = 0;

signals:

public slots:

protected:
	/**
	 * @brief the diagram to which the Element belongs
	 */
	Graph* _graph;
	/**
	 * @brief the main component of the Element shape
	 */
	QAbstractGraphicsShapeItem* _inner;

	/**
	 * @brief the pen used to draw the Element under normal conditions
	 */
	static QPen defaultPen;
	/**
	 * @brief the pen used to draw the Element when highlighted
	 */
	static QPen highlightPen;
	/**
	 * @brief the brush used to paint the Element
	 */
	static QBrush defaultBrush;
};

#endif // ELEMENT_H
