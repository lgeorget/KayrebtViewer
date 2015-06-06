#include "hyperlinkactivatedevent.h"

const QEvent::Type HyperlinkActivatedEvent::HYPERLINK_ACTIVATED_EVENT = static_cast<QEvent::Type>(QEvent::registerEventType(QEvent::User));

HyperlinkActivatedEvent::HyperlinkActivatedEvent(quint64 id, QString url) :
	QEvent(HYPERLINK_ACTIVATED_EVENT), _id(id), _url(url)
{
}

const QString& HyperlinkActivatedEvent::getUrl() const {
	return _url;
}

quint64 HyperlinkActivatedEvent::getId() const {
	return _id;
}

