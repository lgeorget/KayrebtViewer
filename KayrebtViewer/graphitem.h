#ifndef GRAPHITEM_H
#define GRAPHITEM_H

#include <QString>
#include <QFileInfo>
#include <tuple>

class Graph;

class GraphItem
{
public:
	explicit GraphItem(const Graph& graph, GraphItem *parent = 0);
	explicit GraphItem(const QFileInfo& graph, GraphItem *parent = 0);
	GraphItem(GraphItem *parent = 0);
	~GraphItem();

	QString getSymbol();
	QString getDir();
	QString getFile();
	QVariant getValue(int i);
	bool isValid();
	int childrenCount();
	GraphItem* child(int i);

	static constexpr const char* const COLUMNS[] = { "Symbol", "Dir", "File" };
	static constexpr int columnCount() { return 3; }

private:
	QString _symbol;
	QString _dir;
	QString _file;
	bool _valid;
	GraphItem* _parent;
	QList<GraphItem*> _children;

};

#endif // GRAPHITEM_H
