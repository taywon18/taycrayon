#include "chathighlighter.h"

#include <QStack>

ChatHighlighter::ChatHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter (parent)
{
    regenerate();
}

void ChatHighlighter::highlightBlock(const QString &text)
{
    _mutex.lock();
    //setFormat(0, text.count(), defaultFormat);
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
    }
    _mutex.unlock();
}

QString ChatHighlighter::toBBCode(QString text)
{
    _mutex.lock();
    QStringList lines = text.split("\n", QString::SkipEmptyParts);
    QStringList retlines;
    foreach(QString line, lines)
    {
        QString retline = line;
        for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
                QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(line);
                if (!matchIterator.hasNext()) {
                    continue;
                }

                QColor color = rule.bbcodecolor;

                QString colorbegin = "[color="
                        + color.name()
                        + "]";
                QString colorend = "[/color]";
                retline = colorbegin + line + colorend;
        }

        retlines.append(retline);
    }
    _mutex.unlock();

    return retlines.join("\n");
}

void ChatHighlighter::regenerate()
{
    _mutex.lock();
    highlightingRules.clear();
    QFont arial("Arial", 10, QFont::Black);
    arial.setWeight(99);

    QPen outline;
    outline.setStyle(Qt::SolidLine);
    outline.setWidthF(0.7);
    outline.setBrush(Qt::black);
    outline.setCapStyle(Qt::FlatCap);
    outline.setJoinStyle(Qt::MiterJoin);
    outline.setCosmetic(true);

    defaultFormat = QTextCharFormat();
    defaultFormat.setFont(arial);
    if(outlined())
        defaultFormat.setTextOutline(outline);

    defaultFormat.setForeground(QBrush(QColor(255,255,255)));
    // default format
    highlightingRules.append(HighlightingRule(QRegularExpression("^(.+)$"), defaultFormat, QColor(0,0,0)));


    QTextCharFormat pub = defaultFormat;
    pub.setForeground(QBrush(QColor(0,255,0)));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\[PUBLICITÉ\\](.+)$"), pub, QColor(0,255,0)));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] Contact: (.+)$"), pub, QColor(0,255,0)));
    //screen
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] Screenshot Taken - (.+)$"), pub, QColor(0,255,0)));


    QTextCharFormat pm = defaultFormat;
    pm.setForeground(QBrush(QColor(255,255,0)));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\(\\( MP (.+)$"), pm));

    QTextCharFormat radio = defaultFormat;
    radio.setForeground(QBrush(QColor(99,159,255)));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\[Radio\\] (.+)$"), radio));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\[Radio loc\\] (.+)$"), radio));
	highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\(\\( Demande (.+)$"), radio));


    QTextCharFormat admin = defaultFormat;
    admin.setForeground(QBrush(QColor(255,0,0)));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\[OOC\\] (.+)$"), admin));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\[ADMIN\\] (.+)$"), admin));

    QTextCharFormat tel = defaultFormat;
    tel.setForeground(QBrush(QColor(240,230,140)));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\[SMS (.+)$"), tel));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\[Appel (.+)$"), tel));
	highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\[Téléphone\\] (.+)$"), tel));

	QTextCharFormat ooc = defaultFormat;
	ooc.setForeground(QBrush(QColor(200,200,200)));
	highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\(\\( \\[[0-9]+\\] (.+)$"), ooc));



    QTextCharFormat me = defaultFormat;
    me.setForeground(QBrush(QColor(194,162,218)));
    highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] \\* (.+)$"), me));
	highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] Démarrage: (.+)$"), me));
	highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] Utilisez (.+)$"), me));
	highlightingRules.append(HighlightingRule(QRegularExpression("^\\[[0-9]+:[0-9]+:[0-9]+\\] Vous (.+)$"), me));
    _mutex.unlock();
}

bool ChatHighlighter::outlined() const
{
    return _outlined;
}

void ChatHighlighter::setOutlined(bool outlined)
{
    _outlined = outlined;
    regenerate();
}
