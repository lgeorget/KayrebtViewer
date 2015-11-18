#ifndef NODEHOVEREVENT_H
#define NODEHOVEREVENT_H

#include <QEvent>
#include <QString>

class NodeHoverEvent : public QEvent
{
public:
	NodeHoverEvent(int lineNumber, QString& file);
	/**
	 * @brief the identifier of this event class
	 */
	static const QEvent::Type NODE_HOVER_EVENT;

	int getLineNumber() const;
	const QString& getFile() const;

private:
	int _lineNumber;
	QString _file;
};

#endif // NODEHOVEREVENT_H
