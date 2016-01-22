#include "sourcetextviewer.h"
#include "linenumberarea.h"
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QDebug>
#include <QChar>
#include <QPainter>
#include <QColor>
#define CHARACTERS_PER_LINE 122
#define MARGIN_FOR_LINE_NUMBERS 5

const QColor SourceTextViewer::HIGHLIGHTED_LINE_COLOR = QColor(Qt::yellow).lighter(160);
const QColor SourceTextViewer::LINE_NUMBER_AREA_COLOR = QColor(Qt::lightGray).lighter(120);
const QFont SourceTextViewer::MONOSPACE_FONT = QFont("Monospace", 12, QFont::Normal);

SourceTextViewer::SourceTextViewer(QWidget *parent) :
	QPlainTextEdit(parent)
{
	highlighter = new KernelCodeHighlighter();
	document()->setDefaultFont(MONOSPACE_FONT);
//	setCenterOnScroll(true);

	lineNumberArea = new LineNumberArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
	connect(this, SIGNAL(textChanged()), this, SLOT(updateSize()));
	updateLineNumberAreaWidth(0);
}

SourceTextViewer::~SourceTextViewer()
{
	delete highlighter;
}

void SourceTextViewer::highlightLines(int start, int end, bool centerOnScroll)
{
	QList<QTextEdit::ExtraSelection> highlightedLines;

	if (end < start)
		qSwap(end,start);
	for (int line = start - 1 ; line < end ; line++) {
		QTextEdit::ExtraSelection highlightedLine;
		highlightedLine.format.setBackground(HIGHLIGHTED_LINE_COLOR);
		highlightedLine.format.setProperty(QTextFormat::FullWidthSelection, true);
		highlightedLine.cursor = textCursor();
		highlightedLine.cursor.setPosition(document()->findBlockByNumber(line).position());
		highlightedLines.append(highlightedLine);
	}
	setExtraSelections(QList<QTextEdit::ExtraSelection>(highlightedLines));
	gotoLine(start, centerOnScroll);
}

void SourceTextViewer::updateSize()
{
	setMinimumSize(QFontMetrics(document()->defaultFont()).width('m') * CHARACTERS_PER_LINE + lineNumberAreaWidth(), 0);
}

int SourceTextViewer::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}

	int space = MARGIN_FOR_LINE_NUMBERS + fontMetrics().width(QChar('9')) * digits;

	return space;
}

void SourceTextViewer::updateLineNumberAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void SourceTextViewer::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}

void SourceTextViewer::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void SourceTextViewer::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), LINE_NUMBER_AREA_COLOR);

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
	int bottom = top + static_cast<int>(blockBoundingRect(block).height());

	while (block.isValid() && top <= event->rect().bottom()) {
		if (block.isVisible() && bottom >= event->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
							 Qt::AlignHCenter, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + static_cast<int>(blockBoundingRect(block).height());
		++blockNumber;
	}
}

void SourceTextViewer::openSourceFile(const QString& filename)
{
	if (!filename.isEmpty()) {
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			qDebug() << filename;
			QMessageBox::critical(this, tr("Kayrebt::Viewer"), tr("The source file you have selected could not be opened."));
			return;
		}
		setPlainText(file.readAll());
		file.close();
		setDocumentTitle(filename);
		emit titleChanged(filename);
		highlighter->setDocument(document());
	}
}

void SourceTextViewer::gotoLine(int line, bool centerOnCursor)
{
	QTextCursor newCursor = textCursor();
	newCursor.setPosition(document()->findBlockByNumber(line-1).position());
	setTextCursor(newCursor);
	if (centerOnCursor)
		centerCursor();
}
