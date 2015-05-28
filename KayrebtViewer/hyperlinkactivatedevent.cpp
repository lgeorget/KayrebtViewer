#include "hyperlinkactivatedevent.h"

const QEvent::Type HyperlinkActivatedEvent::HYPERLINK_ACTIVATED_EVENT = static_cast<QEvent::Type>(QEvent::registerEventType(QEvent::User));

HyperlinkActivatedEvent::HyperlinkActivatedEvent(QString url) :
	QEvent(HYPERLINK_ACTIVATED_EVENT), _url(url)
{
}

const QString& HyperlinkActivatedEvent::getUrl() {
	return _url;
}
