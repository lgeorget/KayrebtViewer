/**
 * @file databasesortfilterproxymodel.h
 * @author Laurent Georget
 * @date 2015-06-08
 * @brief Definition of class DatabaseSortFilterProxyModel
 */
#ifndef DATABASESORTFILTERPROXYMODEL_H
#define DATABASESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

/**
 * @brief This class is a custom reimplementation of QSortFilterProxyModel that
 * enables multifiltering on the symbol database.
 */
class DatabaseSortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor. Initializes the proxy with no filtering and no
	 * sorting.
	 *
	 * @param parent the parent object
	 */
	DatabaseSortFilterProxyModel(QObject* parent = 0);

public slots:
	/**
	 * @brief Empty redefinition for masking.
	 *
	 * Use one of setSymbolFilterRegExp(), setDirFilterRegExp(), or
	 * setFileFilterRegExp() instead.
	 *
	 * @param unused <i>ignored</i>
	 */
	void setFilterFixedString(const QString & unused); //masked

	/**
	 * @brief Empty redefinition for masking.
	 *
	 * Use one of setSymbolFilterRegExp(), setDirFilterRegExp(), or
	 * setFileFilterRegExp() instead.
	 *
	 * @param unused <i>ignored</i>
	 */
	void setFilterRegExp(const QString & unused); //masked

	/**
	 * @brief Empty redefinition for masking.
	 *
	 * Use one of setSymbolFilterRegExp(), setDirFilterRegExp(), or
	 * setFileFilterRegExp() instead.
	 *
	 * @param unused <i>ignored</i>
	 */
	void setFilterWildcard(const QString & unused); //masked

	/**
	 * @brief Sets the filter for the "Symbol" column.
	 *
	 * @param pattern the regular expression used to filter the "Symbol"
	 * column.
	 */
	void setSymbolFilterRegExp(const QString & pattern);

	/**
	 * @brief Sets the filter for the "Dir" column.
	 *
	 * The strategy used for this regexp is QRegExp::WildcardUnix,
	 * therefore, standard shell wildcards are accepted in this
	 * filter.
	 *
	 * @param pattern the pattern used to filter the "Dir" column.
	 */
	void setDirFilterRegExp(const QString & pattern);

	/**
	 * @brief Sets the filter for the "File" column.
	 *
	 * The strategy used for this regexp is QRegExp::WildcardUnix,
	 * therefore, standard shell wildcards are accepted in this
	 * filter.
	 *
	 * @param pattern the pattern used to filter the "File" column.
	 */
	void setFileFilterRegExp(const QString & pattern);

protected:
	/**
	 * @brief filterAcceptsRow Reimplemented from QSortFilterProxyModel.
	 * Performs the multifiltering on the three columns "Symbol", "Dir",
	 * and "File".
	 * @param source_row the item's row under the source_parent index.
	 * @param source_parent the index of the parent item in the source model.
	 * @return true if the row indexed by source_row under the source_parent
	 * item should be included in the filtered model.
	 */
	bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const override;

private:
	/**
	 * @brief _symbolPattern the "Symbol" column filter.
	 */
	QRegExp _symbolPattern;
	/**
	 * @brief _dirPattern th "Dir" column filter.
	 */
	QRegExp _dirPattern;
	/**
	 * @brief _filePattern the "File" column filter.
	 */
	QRegExp _filePattern;
};

#endif // DATABASESORTFILTERPROXYMODEL_H
