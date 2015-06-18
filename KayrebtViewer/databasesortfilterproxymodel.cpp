/**
 * @file databasesortfilterproxymodel.h
 * @author Laurent Georget
 * @date 2015-06-08
 * @brief Implementation of class DatabaseSortFilterProxyModel
 */
#include "databasesortfilterproxymodel.h"

DatabaseSortFilterProxyModel::DatabaseSortFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent)
{
	_dirPattern.setPatternSyntax(QRegExp::Wildcard);
	_filePattern.setPatternSyntax(QRegExp::Wildcard);
}

void DatabaseSortFilterProxyModel::setFilterFixedString(const QString & ) {} //masked
void DatabaseSortFilterProxyModel::setFilterRegExp(const QString & ) {} //masked
void DatabaseSortFilterProxyModel::setFilterWildcard(const QString & ) {} //masked
void DatabaseSortFilterProxyModel::setSymbolFilterRegExp(const QString & pattern) {
	_symbolPattern.setPattern(pattern);
	invalidateFilter();
}

void DatabaseSortFilterProxyModel::setDirFilterRegExp(const QString & pattern){
	_dirPattern.setPattern(pattern);
	invalidateFilter();
}

void DatabaseSortFilterProxyModel::setFileFilterRegExp(const QString & pattern){
	_filePattern.setPattern(pattern);
	invalidateFilter();
}

bool DatabaseSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex & sourceParent) const {
	QModelIndex indexSymb = sourceModel()->index(sourceRow, 0, sourceParent);
	QModelIndex indexDir = sourceModel()->index(sourceRow, 1, sourceParent);
	QModelIndex indexFile = sourceModel()->index(sourceRow, 2, sourceParent);

	bool ok = true;
	if (ok && !_symbolPattern.isEmpty())
		ok = _symbolPattern.indexIn(sourceModel()->data(indexSymb).toString()) != -1;
	if (ok && !_dirPattern.isEmpty())
		ok = _dirPattern.indexIn(sourceModel()->data(indexDir).toString()) != -1;
	if (ok && !_filePattern.isEmpty())
		ok = _filePattern.indexIn(sourceModel()->data(indexFile).toString()) != -1;

	return ok;
}
