#ifndef SOURCETREEWIDGET_H
#define SOURCETREEWIDGET_H

#include <QTreeView>
#include <QtGui>

class SourceTreeWidget : public QTreeView
{
	Q_OBJECT
public:
	explicit SourceTreeWidget(QWidget *parent = 0);
	~SourceTreeWidget();

signals:
	void filenameSelected(QString dir, QString file);
public slots:
	void selectFile(const QString &file);

private:
	QFileSystemModel* _fs;

private slots:
	void filenameDoubleClicked(const QModelIndex& index);
};

#endif // SOURCETREEWIDGET_H
