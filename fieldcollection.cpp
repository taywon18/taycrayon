#include "fieldcollection.h"

FieldCollection::FieldCollection()
{
}

void FieldCollection::add(QString name, QString format)
{
    (*this)[name] = format;
}
