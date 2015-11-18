/**
 * @file viewer.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class Viewer
 */
#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QListWidget>
#include <QtGui>
#include "graph.h"
#include "sourcetreewidget.h"
#include "databaseviewer.h"
#include "graphitemmodel.h"

namespace Ui {
	/**
	 * @brief This class is the graphical user interface class generated
	 * by QtDesigner.
	 */
	class Viewer;
}

class GraphItem;

/**
 * @brief This is the main window of Kayrebt::Viewer.
 */
class Viewer : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor.
	 *
	 * @param parent the parent widget
	 */
	explicit Viewer(QWidget *parent = 0);
	/**
	 * @brief Destroys the window and its components.
	 */
	~Viewer();
	/**
	 * \brief the context for GraphViz
	 */
	static GVC_t* GRAPHVIZ_CONTEXT;

public slots:
	/**
	 * @brief Opens dialog which lets the user select a diagram to open.
	 */
	void openGraph();
	/**
	 * @brief Opens a diagram in the central pane.
	 *
	 * @param filename the file (in GrpahViz format) containing the graph
	 * to open.
	 */
	void openGraph(const QString &filename);
	void openSourceFile(QMdiSubWindow* window);
	void adaptSourcePanelSize();

signals:
	/**
	 * @brief Triggered when a new diagram is opened.
	 *
	 * @param file the newly built GraphItem
	 */
	void newGraphOpen(const GraphItem& file);

private:
	/**
	 * @brief the graphical interface generated with QtDesigner
	 */
	Ui::Viewer *ui;
	/* *
	 * @brief UNUSED
	 */
	//QDockWidget* _configDock;
	/**
	 * @brief the right pane: the source files tree view
	 */
	QDockWidget* _sourcesDock;
	/**
	 * @brief the left pane: the database and the history
	 */
	QDockWidget* _databaseDock;
	/**
	 * @brief the widget of the @a _sourcesDock
	 */
	SourceTreeWidget* _srcTreeWidget;
	/**
	 * @brief the widget of the @a _databaseDocj
	 */
	DatabaseViewer* _dbviewer;
	/* *
	 * @brief UNUSED
	 */
	//  QList<Drawing *> _scenes;

	/**
	 * @brief the path to the source files of the project under study
	 */
	QString _srcTree;
	/**
	 * @brief the path to the symbol database (a SQLITE3 file) for the
	 * project
	 */
	QString _symbDb;
	/**
	 * @brief the path to the directory where diagrams are generated for
	 * the project
	 */
	QString _diagdir;
	/**
	 * @brief the history containing all diagrams explored, arranged in a
	 * tree
	 */
	GraphItemModel _openGraphs;

	/**
	 * @brief the identifier generator, used to provide GraphItem-s with
	 * identifiers
	 */
	static quint64 _graphsIdGenerator;

	/**
	 * @brief Inner method that actually opens a diagram, instantiates the
	 * graphical components, etc.
	 *
	 * @param file the graph to open
	 *
	 * @return the identifier for the new graph
	 */
	quint64 doOpenGraph(const QFileInfo &file);


protected:
	/**
	 * @brief Handle the HyperlinkActivatedEvent to open a new diagram if
	 * necessary.
	 *
	 * All other events are sent to QMainWindow::event().
	 * Reimplemented from QMainWindow.
	 *
	 * @param event the event
	 *
	 * @return true if the event was accepted and handled
	 */
	virtual bool event(QEvent *event);
};

#endif // VIEWER_H
