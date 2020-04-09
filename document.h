#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QString>
#include <QMap>
#include "generationenvironment.h"

struct FuncCall{
    QString endpoint;
    QStringList args;
};

class Document{
    QString _content;
	QString _fieldSelector, _actionSelector, _funcSelector;
    FieldCollection _fields;
    QList<FuncCall> _calls;

    void regenerateFieldList();

public:
    Document();
    void loadFromString(const QString& str);
    bool loadFromPath(const QString& path);
    QStringList execCalls();

    QString generate(GenerationEnvironment env, QMap<QString, QString> fields = QMap<QString, QString>());
    FieldCollection fields();
};

#endif // DOCUMENT_H
