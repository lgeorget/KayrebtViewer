#ifndef HYPERLINKACTIVATEDEVENT_H
#define HYPERLINKACTIVATEDEVENT_H

#include <QEvent>
#include <QString>

class HyperlinkActivatedEvent : public QEvent
{
public:
	explicit HyperlinkActivatedEvent(quint64 id, QString url);
	const QString& getUrl() const;
	quint64 getId() const;

	static const QEvent::Type HYPERLINK_ACTIVATED_EVENT;

private:
	quint64 _id;
	QString _url;
};

#endif // HYPERLINKACTIVATEDEVENT_H
