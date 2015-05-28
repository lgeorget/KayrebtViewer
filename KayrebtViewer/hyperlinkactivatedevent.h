#ifndef HYPERLINKACTIVATEDEVENT_H
#define HYPERLINKACTIVATEDEVENT_H

#include <QEvent>
#include <QString>

class HyperlinkActivatedEvent : public QEvent
{
public:
	explicit HyperlinkActivatedEvent(QString url);
	const QString& getUrl();

	static const QEvent::Type HYPERLINK_ACTIVATED_EVENT;

private:
	QString _url;
};

#endif // HYPERLINKACTIVATEDEVENT_H
