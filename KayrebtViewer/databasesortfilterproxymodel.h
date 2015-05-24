#ifndef DATABASESORTFILTERPROXYMODEL_H
#define DATABASESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class DatabaseSortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	DatabaseSortFilterProxyModel(QObject* parent = 0);

public slots:
	void setFilterFixedString(const QString & ); //masked
	void setFilterRegExp(const QString & ); //masked
	void setFilterWildcard(const QString & ); //masked
	void setSymbolFilterRegExp(const QString & pattern);
	void setDirFilterRegExp(const QString & pattern);
	void setFileFilterRegExp(const QString & pattern);

protected:
	bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;

private:
	QRegExp _symbolPattern;
	QRegExp _dirPattern;
	QRegExp _filePattern;
};

#endif // DATABASESORTFILTERPROXYMODEL_H
