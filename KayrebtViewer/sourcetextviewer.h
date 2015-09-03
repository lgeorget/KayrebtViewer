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

signals:

public slots:
	void openSourceFile(const QString& filename);

private:
	QSyntaxHighlighter *highlighter;
};

#endif // SOURCETEXTVIEWER_H
