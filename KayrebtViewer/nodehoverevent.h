#ifndef NODEHOVEREVENT_H
#define NODEHOVEREVENT_H

#include <QEvent>

class NodeHoverEvent : public QEvent
{
public:
	NodeHoverEvent(int lineNumber);
	/**
	 * @brief the identifier of this event class
	 */
	static const QEvent::Type NODE_HOVER_EVENT;

	int getLineNumber();

private:
	int _lineNumber;
};

#endif // NODEHOVEREVENT_H
