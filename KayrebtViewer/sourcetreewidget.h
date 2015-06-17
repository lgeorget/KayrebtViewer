/**
 * @file sourcetreewidget.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class SourceTreeWidget
 */
#ifndef SOURCETREEWIDGET_H
#define SOURCETREEWIDGET_H

#include <QTreeView>
#include <QtGui>

/**
 * @brief This class is a tree view of the source code base file hierarchy.
 *
 * It is the right pane in the main window.
 */
class SourceTreeWidget : public QTreeView
{
	Q_OBJECT
public:
	/**
	 * \brief Constructor.
	 *
	 * \param parent the parent widget
	 */
	explicit SourceTreeWidget(QWidget *parent = 0);
	/**
	 * \brief Destroys the widget, and frees its underlying model.
	 */
	~SourceTreeWidget();

signals:
	/**
	 * \brief Triggered when a file is clicked on in the tree view.
	 *
	 * \param dir the directory of the file, relatively to the root of the
	 * displayed tree
	 * \param file the basename of the file
	 */
	void filenameSelected(QString dir, QString file);
public slots:
	/**
	 * \brief Selects a file in the view, scrolling the pane to show it if
	 * necessary.
	 *
	 * \param file the file to select
	 */
	void selectFile(const QString &file);

private:
	/**
	 * \brief the file system model
	 */
	QFileSystemModel* _fs;

private slots:
	/**
	 * \brief Emits the filenameSelected() signal with the information
	 * from the selected file
	 *
	 * \param index the file double-clicked in the tree view
	 */
	void filenameDoubleClicked(const QModelIndex& index);
};

#endif // SOURCETREEWIDGET_H
