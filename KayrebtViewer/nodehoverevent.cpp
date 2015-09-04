#include "nodehoverevent.h"

const QEvent::Type NodeHoverEvent::NODE_HOVER_EVENT = static_cast<QEvent::Type>(QEvent::registerEventType(QEvent::User));

NodeHoverEvent::NodeHoverEvent(int lineNumber) : QEvent(NODE_HOVER_EVENT), _lineNumber(lineNumber)
{
}

int NodeHoverEvent::getLineNumber()
{
	return _lineNumber;
}
