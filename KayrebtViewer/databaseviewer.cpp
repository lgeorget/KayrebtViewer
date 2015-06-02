#include "databaseviewer.h"
#include <QSqlDatabase>
#include "databasesortfilterproxymodel.h"
#include "ui_databaseviewer.h"
#include "historymodel.h"

DatabaseViewer::DatabaseViewer(QWidget *parent) :
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
		_ui->dbView->setSelectionBehavior(QAbstractItemView::SelectRows);
		_ui->dbView->setSortingEnabled(true);
		_ui->dbView->hideColumn(3); // hide line numbers


		// Setup the history view
		_openGraphs = new HistoryModel(_db, this);
		_ui->historyView->setModel(_openGraphs);
		_ui->historyView->verticalHeader()->hide();
		_ui->historyView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_ui->historyView->setSelectionBehavior(QAbstractItemView::SelectRows);
		_ui->historyView->setSortingEnabled(true);
		_ui->historyView->hideColumn(3); // hide line numbers

		connect(_ui->symbolFilter, SIGNAL(textChanged(QString)), _dbFilter, SLOT(setSymbolFilterRegExp(QString)));
		connect(_ui->dirFilter, SIGNAL(textChanged(QString)), _dbFilter, SLOT(setDirFilterRegExp(QString)));
		connect(_ui->fileFilter, SIGNAL(textChanged(QString)), _dbFilter, SLOT(setFileFilterRegExp(QString)));
	}
	connect(_ui->dbView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(symbolDoubleClicked(QModelIndex)));
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

void DatabaseViewer::addGraphToHistory(const QFileInfo& graph)
{
	QSettings settings;

	QString realPath(graph.canonicalFilePath());
	QString prefixPath(settings.value("diagrams dir").toString());

	QRegExp extractor(QRegExp::escape(prefixPath) + "(.*\\/)(.*\\.c)\\/(.*)\\.");
	if (extractor.indexIn(realPath) == -1 || extractor.captureCount() != 3)
		return;

	QStringList captured = extractor.capturedTexts();
	_openGraphs->appendRow({ captured[3], "."+captured[1], captured[2] });
}
