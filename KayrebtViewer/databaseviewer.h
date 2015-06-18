/**
 * @file databaseviewer.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class DatabaseViewer
 */
#ifndef DATABASEVIEWER_H
#define DATABASEVIEWER_H

#include <QtGui>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include "databasesortfilterproxymodel.h"

namespace Ui {
  /**
   * @brief This class is the graphical interface autogenerated by QtDesigner
   */
  class DatabaseViewer;
}

class GraphItem;
class GraphItemModel;

/**
 * @brief This class is the left pane widget of the main view.
 *
 * It contains several tabs. The first one is the view of a database of
 * symbols, the second one the history of explored graphs.
 */
class DatabaseViewer : public QTabWidget
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor. Initializes the left pane widget.
	 *
	 * The history of visited diagrams is actually owned by the main window
	 * and is passed as a parameter to this widget. This is because the
	 * history can also be manipulated from the main window, for example
	 * when a user opens a diagram through the ``File'' menu.
	 *
	 * The constructor is responsible for opening the database of symbols.
	 * If there is an error at this point, the widget is initialized but
	 * the database view remains empty and inactive. The history view is
	 * initialized normally whenever \p history is not null.
	 *
	 * @param history the model containing the history of visited diagrams
	 * @param parent the parent widget, normally the main window
	 */
	explicit DatabaseViewer(GraphItemModel* history, QWidget *parent = 0);

	/**
	 * @brief Closes the database and destroys the widget.
	 */
	~DatabaseViewer();

signals:
	/**
	 * @brief This signal is emitted when a diagram has been selected and
	 * double-clicked by a user in the widget.
	 *
	 * This signal means that the user is expecting the diagram to be shown
	 * in the main window.
	 *
	 * @param graph the path of the diagram to open
	 */
	void graphSelected(QString graph);
	/**
	 * @brief This signal is emitted when a diagram has been selected and
	 * double-clicked by a user in the widget.
	 *
	 * @param symbol the path to the source file where the function
	 * represented by the diagram is implemented
	 */
	void fileSelected(QString symbol);

public slots:
	/**
	 * @brief Updates the field "Dir" and "File" of the filter.
	 *
	 * It is not possible to update only one or the other field for now.
	 * It does not seem useful to programatically update one field
	 * and not the other. Maybe this will change in a future release.
	 *
	 * @param dir the filter regexp for field "Dir"
	 * @param file the filter regexp for field "File"
	 */
	void selectFileAndDirectory(QString dir, QString file);
	/**
	 * @brief Adds a graph to the history of visited graphs.
	 *
	 * \p graph must contain the information about its parent item, in
	 * order to be inserted at the right position in the history tree.
	 *
	 * @param graph the diagram to insert to the history
	 */
	void addGraphToHistory(const GraphItem &graph);
	/**
	 * @brief Opens the history view contextual menu.
	 *
	 * @param point the point (in global coordinates) at which the menu
	 * must be opened
	 */
	void showHistoryContextMenu(const QPoint& point);

private:
	/**
	 * @brief the graphical interface class generated by QtDesigner
	 */
	Ui::DatabaseViewer *_ui;
	/**
	 * @brief the model containing the symbols extracted from the database
	 */
	QSqlTableModel* _db;
	/**
	 * @brief the database of symbols
	 */
	QSqlDatabase _dbBackend;
	/**
	 * @brief the sorter and filter between the SQL model and the view
	 * to enable multifiltering and sorting on the symbols
	 */
	DatabaseSortFilterProxyModel* _dbFilter;
	/**
	 * @brief the history of visited diagrams
	 */
	GraphItemModel* _openGraphs;
	/**
	 * @brief Generalizations from \a databaseSymbolDoubleClicked() and
	 * \a historySymbolDoubleClicked().
	 *
	 * @param model the model (database or history) in which a symbol
	 * was double-clicked
	 * @param index the symbol double-clicked
	 */
	void symbolDoubleClicked(const QAbstractItemModel *model, const QModelIndex& index);

private slots:
	/**
	 * @brief Triggered when an symbol is double-clicked in the database
	 * view.
	 *
	 * @param index the symbol double-clicked.
	 */
	void databaseSymbolDoubleClicked(const QModelIndex& index);
	/**
	 * @brief Triggered when an symbol is double-clicked in the history
	 * view.
	 *
	 * @param index the symbol double-clicked.
	 */
	void historySymbolDoubleClicked(const QModelIndex& index);
};

#endif // DATABASEVIEWER_H
