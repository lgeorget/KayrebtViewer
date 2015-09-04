/**
 * @file drawing.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class Drawing
 */
#ifndef DRAWING_H
#define DRAWING_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

class Graph;

/**
 * @brief This class represents the drawing area where a diagram is shown.
 */
class Drawing : public QGraphicsView
{
	Q_OBJECT

public:
	/**
	 * @brief Unique constructor.
	 *
	 * This function builds a Drawing instance with a given unique id (no
	 * other open Drawing instance should have the same id), where the
	 * diagram contained in \p filename is drawn.
	 *
	 * @param id the identifier for this Drawing (and the diagram shown on it)
	 * @param filename the file where the diagram to draw is stored
	 * @param parent the parent widget, should be the main window in most
	 * cases
	 */
	explicit Drawing(quint64 id, QString filename, QWidget *parent = 0);
	/**
	 * @brief Destroys the Drawing and the diagram shown on it.
	 */
	~Drawing();
	/**
	 * @brief Gives the identifier of the Drawing (actually the id of its diagram).
	 * @return the identifier of the diagram shown on the Drawing
	 */
	qint64 getId() const;
	/**
	 * @brief Gives the diagram displayed by the Drawing.
	 * @return the diagram shown on the Drawing
	 */
	const Graph* getGraph() const { return _graph; }

public slots:
	/**
	 * @brief Requests the contextual menu to be displayed.
	 *
	 * @param point the point (in global coordinates) where the contectual
	 * menu should appear
	 */
	void showContextMenu(const QPoint& point);

protected:
	/**
	 * @brief Reacts to a mouse wheel event by adjusting the zoom.
	 *
	 * The mouse wheel is used to zoom in and out in the Drawing. With key
	 * Ctrl pressed, the zoom is centered on the widget center, otherwise
	 * it is centered on the mouse position.
	 *
	 * @param event the event received from the mouse
	 */
	virtual void wheelEvent(QWheelEvent *event);

private:
	/**
	 * @brief the diagram displayed on the Drawing
	 */
	Graph *_graph;
};

#endif // DRAWING_H
