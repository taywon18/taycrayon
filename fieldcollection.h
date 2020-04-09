#ifndef FIELDCOLLECTION_H
#define FIELDCOLLECTION_H

#include <QMap>
#include <QString>

class FieldCollection
        : public QMap<QString, QString>
{
public:
    FieldCollection();
    void add(QString name, QString format);
};

#endif // FIELDCOLLECTION_H
