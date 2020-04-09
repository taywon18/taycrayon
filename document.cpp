#include "document.h"

#include <QFile>
#include <QUrl>
#include <QSet>
#include <QTextStream>
#include <QDesktopServices>
#include <QRegularExpression>

Document::Document()
	: _fieldSelector("\\$\\{([ a-zA-Z0-9\\.]+):?([ a-zA-Z0-9\\.,éè\\(\\)]+)?\\}")
    , _actionSelector("£{([a-zA-Z0-9]+)\\((?:([.?=\\-&a-zA-Z0-9_:\\/\\-]+),?)*\\)}")
{

}

void Document::loadFromString(const QString &str)
{
    _content = str;
    regenerateFieldList();
}

bool Document::loadFromPath(const QString &path)
{
    if(!QFile::exists(path))
        return false;

    QFile reader(path);
    if(!reader.open(QFile::ReadOnly | QFile::Text))
        return false;
    QTextStream in(&reader);
    QString buff = in.readAll();
    reader.close();

    loadFromString(buff);
    return true;
}

QStringList Document::execCalls()
{
    QStringList errors;
    for(auto fc : _calls)
    {
        if(fc.endpoint == "ouvrirUrl")
        {
            if(fc.args.size() != 1)
            {
                errors.append(QString("Mauvais nombre d'arguments pour l'endpoint %1: %0.").arg(QString::number(fc.args.size()), fc.endpoint));
                continue;
            }

            QDesktopServices::openUrl ( QUrl(fc.args[0]) );
        }
        else
            errors.append(QString("endpoint inconnu: %0.").arg(fc.endpoint));
    }

    return errors;
}

QString Document::generate(GenerationEnvironment env, QMap<QString, QString> fields)
{
    QString ret = _content;

    QRegExp regexp(_fieldSelector);
    int pos = 0;

    while (pos >= 0) {
        pos = regexp.indexIn(ret, pos);
        if (pos >= 0) {
            int length = regexp.matchedLength();
            auto caps = regexp.capturedTexts();

            QString r;
            if(fields.contains(caps[1])){
                r = fields[caps[1]];
            }
            else {
                if(caps.length()-2)
                    r = env.parse(caps[1], caps[2]);
                else
                    r = env.parse(caps[1]);
            }



            ret.remove(pos, length);
            ret.insert(pos, r);
        }
    }

    _content.remove(QRegularExpression(_actionSelector));
    return ret;
}

FieldCollection Document::fields()
{
    return _fields;
}

void Document::regenerateFieldList()
{
    _fields.clear();
    _calls.clear();

    QRegExp regexp(_fieldSelector);
    int pos = 0;
    while (pos >= 0)
    {
        pos = regexp.indexIn(_content, pos);
        if (pos >= 0)
        {
            auto caps = regexp.capturedTexts();

            QString name, format;

            name = caps[1];
            if(caps.length() > 2)
                format = caps[2];

            _fields.add(name, format);

            pos++;
        }
    }

	QRegularExpression regexp2 = QRegularExpression(_actionSelector);
    QRegularExpressionMatchIterator i = regexp2.globalMatch(_content);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();

        auto caps = match.capturedTexts();

        QString endpointName = caps[1];
        QStringList args = caps.mid(1);
        args.removeAt(0);		

        FuncCall fc;
        fc.endpoint = endpointName;
        fc.args = args;
        _calls.append(fc);
    }
}
