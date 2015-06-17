/**
 * @file hyperlinkactivatedevent.h
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Definition of class HyperlinkActivatedEvent
 */
#ifndef HYPERLINKACTIVATEDEVENT_H
#define HYPERLINKACTIVATEDEVENT_H

#include <QEvent>
#include <QString>

/**
 * @brief This class represents an event which is sent when a user activates
 * an hyperlink.
 *
 * It is used when a user clicks on a function node in a diagram to open the
 * diagram of the function called. The event carries the identifier of the
 * diagram the event is sent from as well as the URL embedded in the function
 * call node.
 */
class HyperlinkActivatedEvent : public QEvent
{
public:
	/**
	 * @brief Constructor. Builds an event from the sender's id and an URL
	 *
	 * @param id the identifier of the sender
	 * @param url the URL of the hyperlink
	 */
	explicit HyperlinkActivatedEvent(quint64 id, QString url);
	/**
	 * @brief Gets the URL carried by the event
	 *
	 * @return the URL carried by the event
	 */
	const QString& getUrl() const;
	/**
	 * @brief Gets the id of the event sender
	 *
	 * @return the id of the event sender
	 */
	quint64 getId() const;

	/**
	 * @brief the identifier of this event class
	 */
	static const QEvent::Type HYPERLINK_ACTIVATED_EVENT;

private:
	/**
	 * @brief the id of the sender
	 */
	quint64 _id;
	/**
	 * @brief the url of the hyperlink
	 */
	QString _url;
};

#endif // HYPERLINKACTIVATEDEVENT_H
