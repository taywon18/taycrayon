#ifndef CHATHIGHLIGHTER_H
#define CHATHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextFormat>
#include <QMutex>


class ChatHighlighter : public QSyntaxHighlighter
{

public:
    ChatHighlighter(QTextDocument *parent);
    void highlightBlock(const QString &text);
    QString toBBCode(QString text);


    bool outlined() const;
    void setOutlined(bool outlined);

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
        QColor bbcodecolor;

        inline HighlightingRule(QRegularExpression p, QTextCharFormat f, QColor col){
            pattern = p; format = f; bbcodecolor = col;
        }

        inline HighlightingRule(QRegularExpression p, QTextCharFormat f){
            pattern = p; format = f; bbcodecolor = f.foreground().color();
        }
    };
    void regenerate();
    QList<HighlightingRule> highlightingRules;
    QTextCharFormat defaultFormat;
    bool _outlined = true;
    QMutex _mutex;
};

#endif // CHATHIGHLIGHTER_H
