#ifndef GRAPHITEM_H
#define GRAPHITEM_H

#include <QString>
#include <QFileInfo>
#include <tuple>

class Graph;

class GraphItem
{
public:
	explicit GraphItem(const Graph& graph, quint64 id, GraphItem *parent = 0);
	explicit GraphItem(const QFileInfo& graph, quint64 id, GraphItem *parent = 0);
	GraphItem(const GraphItem& graph);
	GraphItem() = default;
	~GraphItem();

	QString getSymbol() const;
	QString getDir() const;
	QString getFile() const;
	quint64 getId() const;
	GraphItem *getParent() const;
	QVariant getValue(int i) const;

	void setSymbol(QString symbol);
	void setDir(QString dir);
	void setFile(QString file);
	void setId(quint64 id);
	bool setValue(int i, QVariant v);

	int childrenCount() const;
	GraphItem* child(int i) const;
	int childNumber() const;

	void addChild(int i, const GraphItem& newChild);
	bool removeChild(int i);

	static constexpr const char* const COLUMNS[] = { "Symbol", "Dir", "File" };
	static constexpr int columnCount() { return 3; }

private:
	quint64 _id = 0;
	QString _symbol;
	QString _dir;
	QString _file;
	GraphItem* _parent;
	QList<GraphItem*> _children;
};

#endif // GRAPHITEM_H
