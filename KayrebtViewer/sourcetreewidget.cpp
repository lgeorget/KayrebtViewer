/**
 * @file sourcetreewidget.cpp
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Implementation of class SourceTreeWidget
 */
#include "sourcetreewidget.h"
#include <QtGui>

SourceTreeWidget::SourceTreeWidget(QWidget *parent) :
	QTreeView(parent)
{
	QString dir = QSettings().value("source tree").toString();
	_fs = new QFileSystemModel;
	_fs->setRootPath(dir);
	_fs->setNameFilters({"*.h","*.c"});
	_fs->setNameFilterDisables(false);

	setModel(_fs);
	setRootIndex(_fs->index(dir));
	for (int i = 1 ; i < _fs->columnCount() ; i++)
		setColumnHidden(i, true); // mask all but first column

	connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(filenameDoubleClicked(QModelIndex)));
}

SourceTreeWidget::~SourceTreeWidget()
{
	_fs->deleteLater();
}

void SourceTreeWidget::filenameDoubleClicked(const QModelIndex& index) {
	QFileInfo selection(_fs->rootDirectory().relativeFilePath(_fs->filePath(index)));
	if (_fs->isDir(index)) {
		emit filenameSelected(selection.filePath(), QString());
	} else {
		emit filenameSelected(selection.path(), selection.fileName());
	}
}

void SourceTreeWidget::selectFile(const QString& file) {
	qDebug() << "selecting " << file;
	collapseAll();

	QModelIndex index = _fs->index(file);
	expand(index);
	setCurrentIndex(index);
}
