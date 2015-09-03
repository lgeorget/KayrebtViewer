#ifndef KERNELCODEHIGHLIGHTER_H
#define KERNELCODEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegExp>
#include <QVector>

class QTextDocument;

class KernelCodeHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	KernelCodeHighlighter(QTextDocument *parent = 0);

protected:
	void highlightBlock(const QString &text);

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat operatorFormat;
	QTextCharFormat typeFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat preprocessorFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat numberFormat;
	QTextCharFormat functionFormat;
};

#endif // KERNELCODEHIGHLIGHTER_H
