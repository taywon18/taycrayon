#ifndef BINDCONTAINER_H
#define BINDCONTAINER_H

#include <QVariant>
#include <QList>
#include "bind.h"

class BindContainer : public QList<Bind>
{

public:
    BindContainer(QVariant lst = QVariant());
    QVariantList toQVariantList();
};

#endif // BINDCONTAINER_H
