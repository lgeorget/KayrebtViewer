/**
 * @file graphitem.cpp
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Implementation of class GraphItem
 */
#include "graphitem.h"
#include <QSettings>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include "graph.h"

constexpr const char* const GraphItem::COLUMNS[];

GraphItem::GraphItem(const Graph& graph, quint64 id, GraphItem *parent) : GraphItem(QFileInfo(graph.getFilename()), id, parent)
{
}

GraphItem::GraphItem(const QFileInfo& graph, quint64 id, GraphItem *parent) : _id(id), _parent(parent)
{
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

	qDebug() << "Created graph item " << _id;
}

GraphItem::GraphItem(const GraphItem& graph) : _id(graph._id), _symbol(graph._symbol), _dir(graph._dir),
												_file(graph._file), _parent(graph._parent),
												_children(QList<GraphItem*>())
{
	for (GraphItem* g : graph._children)
		_children.append(new GraphItem(*g)); // deep copy
}

GraphItem::~GraphItem()
{
	qDeleteAll(_children);
}

QString GraphItem::getSymbol() const
{
	return _symbol;
}

QString GraphItem::getDir() const
{
	return _dir;
}

QString GraphItem::getFile() const
{
	return _file;
}

quint64 GraphItem::getId() const
{
	return _id;
}

void GraphItem::setSymbol(QString symb)
{
	_symbol = symb;
}

void GraphItem::setDir(QString dir)
{
	_dir = dir;
}

void GraphItem::setFile(QString file)
{
	_file = file;
}

QVariant GraphItem::getValue(int i) const
{
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


bool GraphItem::setValue(int i, QVariant v)
{
	switch (i) {
		case 0:
			_symbol = v.toString();
			return true;
		case 1:
			_dir = v.toString();
			return true;
		case 2:
			_file = v.toString();
			return true;
		default:
			return false;
	}
}


GraphItem *GraphItem::getParent() const
{
	return _parent;
}

int GraphItem::childrenCount() const
{
	return _children.count();
}

GraphItem* GraphItem::child(int i) const
{
	return _children.value(i);
}

int GraphItem::childNumber() const
{
	if (_parent)
		return _parent->_children.indexOf(const_cast<GraphItem*>(this));
	return 0;
}

void GraphItem::addChild(int i, const GraphItem& newChild)
{
	if (i > _children.count())
		i = _children.count();

	GraphItem* newItem = new GraphItem(newChild);
	newItem->_parent = this;
	_children.insert(i, newItem);
}

bool GraphItem::removeChild(int i)
{
	if (i >= _children.count() || i < 0)
		return false;

	GraphItem* it = _children.at(i);
	_children.removeAt(i);
	delete it;

	return true;
}
