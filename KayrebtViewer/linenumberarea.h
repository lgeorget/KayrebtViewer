#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "sourcetextviewer.h"

class LineNumberArea : public QWidget
{
public:
	LineNumberArea(SourceTextViewer *viewer) : QWidget(viewer), _textViewer(viewer) {
		this->setFont(_textViewer->document()->defaultFont());
	}

	QSize sizeHint() const {
		return QSize(_textViewer->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event) {
		_textViewer->lineNumberAreaPaintEvent(event);
	}

private:
	SourceTextViewer *_textViewer;
};

#endif // LINENUMBERAREA_H
