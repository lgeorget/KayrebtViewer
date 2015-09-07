#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "sourcetextviewer.h"

class LineNumberArea : public QWidget
{
public:
	LineNumberArea(SourceTextViewer *viewer) : QWidget(viewer) {
		textViewer = viewer;
	}

	QSize sizeHint() const {
		return QSize(textViewer->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event) {
		textViewer->lineNumberAreaPaintEvent(event);
	}

private:
	SourceTextViewer *textViewer;
};

#endif // LINENUMBERAREA_H
