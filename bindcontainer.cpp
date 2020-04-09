#include "bindcontainer.h"
#include <QVariant>

BindContainer::BindContainer(QVariant var)
{
    auto lst = var.toList();
    foreach(QVariant i, lst)
        push_back(Bind(i));
}

QVariantList BindContainer::toQVariantList()
{
    QVariantList lst;
    for(Bind i : *this)
        lst.push_back(i.toVariantMap());

    return lst;
}
