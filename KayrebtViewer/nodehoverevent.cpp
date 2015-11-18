#include "nodehoverevent.h"

const QEvent::Type NodeHoverEvent::NODE_HOVER_EVENT = static_cast<QEvent::Type>(QEvent::registerEventType(QEvent::User));

NodeHoverEvent::NodeHoverEvent(int lineNumber, QString& file) : QEvent(NODE_HOVER_EVENT), _lineNumber(lineNumber), _file(file)
{
}

int NodeHoverEvent::getLineNumber() const
{
	return _lineNumber;
}

const QString& NodeHoverEvent::getFile() const
{
	return _file;
}
