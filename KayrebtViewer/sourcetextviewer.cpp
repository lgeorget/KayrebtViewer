#include "sourcetextviewer.h"
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QDebug>

const QFont SourceTextViewer::MONOSPACE_FONT = QFont("Monospace", 12, QFont::Normal);

SourceTextViewer::SourceTextViewer(QWidget *parent) :
	QPlainTextEdit(parent)
{
	highlighter = new KernelCodeHighlighter();
	document()->setDefaultFont(MONOSPACE_FONT);
	setCenterOnScroll(true);
}

SourceTextViewer::~SourceTextViewer()
{
	delete highlighter;
}

void SourceTextViewer::openSourceFile(const QString& filename)
{
	if (!filename.isEmpty()) {
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QMessageBox::critical(this, tr("Kayrebt::Viewer"), tr("The source file you have selected could not be open."));
			return;
		}
		setPlainText(file.readAll());
		setDocumentTitle(filename);
		highlighter->setDocument(document());
	}
}

void SourceTextViewer::gotoLine(int line)
{
	QTextCursor newCursor = textCursor();
	newCursor.setPosition(document()->findBlockByLineNumber(line-1).position());
	setTextCursor(newCursor);
}
