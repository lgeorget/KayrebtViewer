#include "databaseviewer.h"
#include <QSqlDatabase>
#include "databasesortfilterproxymodel.h"

DatabaseViewer::DatabaseViewer(QWidget *parent) :
	QWidget(parent)
{
	QString dbFile = QSettings().value("symbol database").toString();
	_dbView = new QTableView(this);
	_filterName = new QLineEdit(this);
	_filterDir = new QLineEdit(this);
	_filterFile = new QLineEdit(this);
	QHBoxLayout* filters = new QHBoxLayout;
	filters->addWidget(_filterName);
	filters->addWidget(_filterDir);
	filters->addWidget(_filterFile);
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(filters);
	mainLayout->addWidget(_dbView);
	setLayout(mainLayout);

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
		_dbView->setModel(_dbFilter);

		_dbView->verticalHeader()->hide();
		_dbView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_dbView->setSelectionBehavior(QAbstractItemView::SelectRows);
		_dbView->setSortingEnabled(true);
		_dbView->hideColumn(3); // hide line numbers

		connect(_filterName, SIGNAL(textChanged(QString)), _dbFilter, SLOT(setSymbolFilterRegExp(QString)));
		connect(_filterDir, SIGNAL(textChanged(QString)), _dbFilter, SLOT(setDirFilterRegExp(QString)));
		connect(_filterFile, SIGNAL(textChanged(QString)), _dbFilter, SLOT(setFileFilterRegExp(QString)));
	}
	connect(_dbView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(symbolDoubleClicked(QModelIndex)));
}

DatabaseViewer::~DatabaseViewer()
{
	if (_dbBackend.isOpen())
		_dbBackend.close();
}

void DatabaseViewer::selectFileAndDirectory(QString dir, QString file)
{
	_filterDir->setText(dir);
	_filterFile->setText(file);
}


void DatabaseViewer::symbolDoubleClicked(const QModelIndex& index)
{
	QSettings settings;
	int row = index.row();
	QString file = settings.value("source tree").toString() +
			_dbFilter->sibling(row, 1, index).data().toString() + "/" +
			_dbFilter->sibling(row, 2, index).data().toString();
	emit fileSelected(file);

	QString graph = settings.value("diagrams dir").toString() +
			_dbFilter->sibling(row, 1, index).data().toString() + "/" +
			_dbFilter->sibling(row, 2, index).data().toString() + "/" +
			_dbFilter->sibling(row, 0, index).data().toString() + ".dot";
	emit symbolSelected(graph);
}
