#ifndef SOURCETEXTVIEWER_H
#define SOURCETEXTVIEWER_H

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include "kernelcodehighlighter.h"

class SourceTextViewer : public QPlainTextEdit
{
	Q_OBJECT
public:
	explicit SourceTextViewer(QWidget *parent = 0);
	~SourceTextViewer();
	static const QFont MONOSPACE_FONT;
	void gotoLine(int line, bool centerOnCursor = true);

	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();
	QSize sizeHint();

protected:
	void resizeEvent(QResizeEvent *e);

signals:

public slots:
	void openSourceFile(const QString& filename);
	void highlightLines(int start, int end, bool centerOnScroll = true);

private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void updateLineNumberArea(const QRect &, int);
	void updateSize();

private:
	QSyntaxHighlighter *highlighter;
	QWidget *lineNumberArea;
	static const QColor HIGHLIGHTED_LINE_COLOR;
	static const QColor LINE_NUMBER_AREA_COLOR;
};

#endif // SOURCETEXTVIEWER_H
