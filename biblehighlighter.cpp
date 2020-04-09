#include "biblehighlighter.h"
#include <QRegularExpression>

BibleHighlighter::BibleHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter (parent)
{
    QTextCharFormat tmplformat;
    tmplformat.setForeground(QBrush(QColor(0,0,255)));
    tmplformat.setFontWeight(QFont::Bold);

    QTextCharFormat actionformat;
    actionformat.setForeground(QBrush(QColor(255,0,0)));
    actionformat.setFontWeight(QFont::Bold);

    QTextCharFormat bbcodeformat;
    bbcodeformat.setForeground(QBrush(Qt::gray));
    bbcodeformat.setFontWeight(QFont::Thin);


    highlightingRules.append(HighlightingRule(QRegularExpression("\\[[/]?[.a-zA-Z0-9éèà\"=:#; /\\()-\\?]+\\]"), bbcodeformat));
    highlightingRules.append(HighlightingRule(QRegularExpression("<[/]?[.a-zA-Z0-9%\"=:#; /\\()-\\?]+>"), bbcodeformat));
	highlightingRules.append(HighlightingRule(QRegularExpression("\\$\\{([ a-zA-Z0-9\\.,é]+):?([a-zA-Z0-9\\.,éèà\\(\\)]+)?\\}"), tmplformat));
	highlightingRules.append(HighlightingRule(QRegularExpression("£{([a-zA-Z0-9]+)\\((?:([.?=\\-&a-zA-Z0-9_:\\/\\-\\(\\)]+),?)*\\)}"), actionformat));

}


void BibleHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
}
