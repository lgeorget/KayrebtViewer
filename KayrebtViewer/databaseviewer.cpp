/**
 * @file databaseviewer.cpp
 * @author Laurent Georget
 * @date 2015-06-17
 */
#include <QtGui>
#include <QSqlDatabase>
#include "databasesortfilterproxymodel.h"
#include "ui_databaseviewer.h"
#include "graphitemmodel.h"
#include "graphitem.h"
#include "databaseviewer.h"

DatabaseViewer::DatabaseViewer(GraphItemModel *history, QWidget *parent) :
	QTabWidget(parent),
	_ui(new Ui::DatabaseViewer)
{
	_ui->setupUi(this);

	QString dbFile = QSettings().value("symbol database").toString();
	_dbBackend = QSqlDatabase::addDatabase("QSQLITE");
	_dbBackend.setDatabaseName(dbFile);
	_dbFilter = new DatabaseSortFilterProxyModel(this);

	if (_dbBackend.open()) {
		_db = new QSqlTableModel(this, _dbBackend);
		_db->setTable("global_symbols");
		_db->setHeaderData(0, Qt::Horizontal, tr("Function name"));
		_db->setHeaderData(1, Qt::Horizontal, tr("Directory"));
		_db->setHeaderData(2, Qt::Horizontal, tr("File"));
		_db->select();

		_dbFilter->setSourceModel(_db);
		while (_db->canFetchMore()) { // this may be wrong, TODO: find a way to use
			_db->fetchMore();         // the filtering proxy with incremental data
		}
		_ui->dbView->setModel(_dbFilter);

		_ui->dbView->verticalHeader()->hide();
		_ui->dbView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_ui->dbView->setSortingEnabled(true);
		_ui->dbView->hideColumn(3); // hide line numbers

		connect(_ui->symbolFilter, SIGNAL(textChanged(QString)), _dbFilter, SLOT(setSymbolFilterRegExp(QString)));
		connect(_ui->dirFilter, SIGNAL(textChanged(QString)), _dbFilter, SLOT(setDirFilterRegExp(QString)));
		connect(_ui->fileFilter, SIGNAL(textChanged(QString)), _dbFilter, SLOT(setFileFilterRegExp(QString)));
		connect(_ui->dbView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(databaseSymbolDoubleClicked(QModelIndex)));
	}

	// Setup the history view
	_openGraphs = history;
	if (_openGraphs) {
		_ui->historyView->setModel(_openGraphs);
		_ui->historyView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_ui->historyView->setSortingEnabled(false);
		_ui->historyView->hideColumn(3); // hide line numbers
		_ui->historyView->setContextMenuPolicy(Qt::CustomContextMenu);

		connect(_ui->historyView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(historySymbolDoubleClicked(QModelIndex)));
		connect(_ui->historyView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showHistoryContextMenu(QPoint)));
	}
}

DatabaseViewer::~DatabaseViewer()
{
	if (_dbBackend.isOpen())
		_dbBackend.close();
	delete _ui;
}

void DatabaseViewer::selectFileAndDirectory(QString dir, QString file)
{
	_ui->dirFilter->setText(dir);
	_ui->fileFilter->setText(file);
}


void DatabaseViewer::symbolDoubleClicked(const QAbstractItemModel* model, const QModelIndex& index)
{
	QSettings settings;
	int row = index.row();
	QString file = settings.value("source tree").toString() +
			model->sibling(row, 1, index).data().toString() + "/" +
			model->sibling(row, 2, index).data().toString();
	emit fileSelected(file);

	QString graph = settings.value("diagrams dir").toString() +
			model->sibling(row, 1, index).data().toString() + "/" +
			model->sibling(row, 2, index).data().toString() + "/" +
			model->sibling(row, 0, index).data().toString() + ".dot";
	emit graphSelected(graph);
}

void DatabaseViewer::databaseSymbolDoubleClicked(const QModelIndex& index)
{
	symbolDoubleClicked(_dbFilter, index);
}

void DatabaseViewer::historySymbolDoubleClicked(const QModelIndex& index)
{
	symbolDoubleClicked(_openGraphs, index);
}

void DatabaseViewer::addGraphToHistory(const GraphItem& graph)
{
	_openGraphs->appendGraphItem(graph);
}

void DatabaseViewer::showHistoryContextMenu(const QPoint& point)
{
	QModelIndex index = _ui->historyView->indexAt(point);
	if (!index.isValid())
		return;

	QMenu contextMenu;
	QAction removeGraph(tr("Delete"), this);
	contextMenu.addAction(&removeGraph);

	QAction* chosen = contextMenu.exec(_ui->historyView->mapToGlobal(point));
	if (chosen == &removeGraph) {
		_openGraphs->removeRow(index.row(), index.parent());
	}
}
