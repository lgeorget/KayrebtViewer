#include "graphitem.h"
#include <QSettings>
#include <QRegExp>
#include <QStringList>
#include "graph.h"

constexpr const char* const GraphItem::COLUMNS[];

GraphItem::GraphItem(const Graph& graph, GraphItem *parent) : GraphItem(QFileInfo(graph.getFilename()), parent)
{
}

GraphItem::GraphItem(const QFileInfo& graph, GraphItem *parent) : _parent(parent)
{
	if (!graph.exists()) {
		_valid = false;
	}

	QSettings settings;
	QString realPath(graph.canonicalFilePath());
	QString prefixPath(settings.value("diagrams dir").toString());

	QRegExp extractor(QRegExp::escape(prefixPath) + "(.*\\/)(.*\\.c)\\/(.*)\\.");
	if (extractor.indexIn(realPath) == -1 || extractor.captureCount() != 3)
		return;

	QStringList captured = extractor.capturedTexts();
	_symbol = captured[3];
	_dir = "./" + captured[1];
	_file = captured[2];
	_valid = true;
}

GraphItem::GraphItem(GraphItem *parent) : _valid(false), _parent(parent)
{
}

GraphItem::~GraphItem()
{
	qDeleteAll(_children);
}

QString GraphItem::getSymbol()
{
	return _symbol;
}

QString GraphItem::getDir()
{
	return _dir;
}

QString GraphItem::getFile()
{
	return _file;
}

QVariant GraphItem::getValue(int i)
{
	if (!isValid())
		return QVariant();

	switch (i) {
		case 0:
			return _symbol;
		case 1:
			return _dir;
		case 2:
			return _file;
		default:
			return QVariant();
	}
}

bool GraphItem::isValid()
{
	return isValid();
}


int GraphItem::childrenCount()
{
	return _children.count();
}

GraphItem* GraphItem::child(int i)
{
	return _children.value(i);
}
