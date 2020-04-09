#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class QTextDocument;

class BibleHighlighter : public QSyntaxHighlighter
{
public:
    BibleHighlighter(QTextDocument *parent);
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;

        inline HighlightingRule(QRegularExpression p, QTextCharFormat f){
            pattern = p; format = f;
        }
    };
    QList<HighlightingRule> highlightingRules;

};

#endif // HIGHLIGHTER_H
