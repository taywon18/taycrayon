#ifndef BIND_H
#define BIND_H
#include <QString>
#include <QKeySequence>
#include <QList>
#include <QVariant>
#include "bindpossibility.h"



class Bind
{
    QString _name;
    bool _activated = true;
    QKeySequence _keySequence;
    QList<BindPossibility> _possibilities;

public:
    Bind(QVariant var = QVariant());
    QVariantMap toVariantMap();
    QString name() const;
    void setName(const QString &name);
    bool activated() const;
    void setActivated(bool activated);
    QKeySequence keySequence() const;
    void setKeySequence(const QKeySequence &keySequence);
    QList<BindPossibility>& possibilities();
};

#endif // BIND_H
