#include "kernelcodehighlighter.h"
#include <QFont>
#include <QTextDocument>
#include <QStringList>
#include <QString>

/* Code copy-pasted-adapted from http://doc.qt.io/qt-4.8/qt-richtext-syntaxhighlighter-example.html */

KernelCodeHighlighter::KernelCodeHighlighter(QTextDocument *parent) :
	QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	keywordFormat.setForeground(Qt::darkCyan);
	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\bif\\b" << "\\belse\\b"
					<< "\\bswitch\\b" << "\\bcase\\b"
					<< "\\bfor\\b" << "\\bgoto\\b"
					<< "\\bwhile\\b" << "\\bdo\\b"
					<< "\\binline\\b"
					<< "\\btypedef\\b"
					<< "\\breturn\\b"
					<< "\\bsizeof\\b";
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	typeFormat.setForeground(Qt::darkYellow);
	typeFormat.setFontWeight(QFont::Bold);
	QStringList typePatterns;
	typePatterns  << "\\bint\\b" << "\\blong\\b" << "\\bshort\\b"
					<< "\\bchar\\b"
					<< "\\bvoid\\b"
					<< "\\bfloat\\b" << "\\bdouble\\b"
					<< "\\bsigned\\b" << "\\bunsigned\\b"
					<< "\\bstatic\\b"
					<< "\\bconst\\b" << "\\bvolatile\\b"
					<< "\\benum\\b" << "\\bstruct\\b"<< "\\bunion\\b"
					<< "\\b[A-Za-z0-9_]+_t\\b";
	foreach (const QString &pattern, typePatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = typeFormat;
		highlightingRules.append(rule);
	}

	operatorFormat.setForeground(Qt::darkRed);
	QStringList operatorPatterns;
	operatorPatterns  << "\\+" << "\\-" << "\\{" << "\\}" << "\\(" << "\\)"
				  << "\\*" << "\\/" << "\\<" << "\\>" << "\\=" << "\\^"
				  << "\\%" << "\\!" << "\\?" << "\\[" << "\\]" << "\\~"
				  << "\\&" << "\\|" << "\\:" << "\\." << "\\;";
	foreach (const QString &pattern, operatorPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = typeFormat;
		highlightingRules.append(rule);
	}

	quotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	numberFormat.setForeground(Qt::magenta);
	rule.pattern = QRegExp("\\b[0-9]+\\.[0-9]*\\b");
	rule.format = numberFormat;
	highlightingRules.append(rule);

	functionFormat.setForeground(Qt::darkBlue);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\(.*)");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::gray);
	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");

	preprocessorFormat.setForeground(Qt::darkRed);
	rule.pattern = QRegExp("\\#[a-zA-Z]+\\b");
	rule.format = preprocessorFormat;
	highlightingRules.append(rule);
}

void KernelCodeHighlighter::highlightBlock(const QString &text)
{
	foreach (const HighlightingRule &rule, highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = commentStartExpression.indexIn(text);
	while (startIndex >= 0) {
		int endIndex = commentEndExpression.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex
							+ commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
	}
}

