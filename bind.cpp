#include "bind.h"

QString Bind::name() const
{
    return _name;
}

void Bind::setName(const QString &name)
{
    _name = name;
}

bool Bind::activated() const
{
    return _activated;
}

void Bind::setActivated(bool activated)
{
    _activated = activated;
}

QKeySequence Bind::keySequence() const
{
    return _keySequence;
}

void Bind::setKeySequence(const QKeySequence &keySequence)
{
    _keySequence = keySequence;
}

QList<BindPossibility>& Bind::possibilities()
{
    if(_possibilities.empty())
        _possibilities.append(BindPossibility());

    return _possibilities;
}

Bind::Bind(QVariant var)
{
    QVariantMap obj = var.toMap();
    if(obj.isEmpty())
        return;
    
    _name = obj.value("name", _name).toString();
    _activated = obj.value("activated", _activated).toBool();
    _keySequence = QKeySequence(obj.value("keySequence", _keySequence.toString(QKeySequence::PortableText)).toString(), QKeySequence::PortableText);
    foreach(QVariant v, obj.value("possibilities", QVariantList()).toList())
        _possibilities.push_back(BindPossibility(v));

    if(_possibilities.empty())
        _possibilities.append(BindPossibility());
}

QVariantMap Bind::toVariantMap()
{
    QVariantMap ret;
    ret["name"] = _name;
    ret["keySequence"] = _keySequence.toString(QKeySequence::PortableText);
    ret["activated"] = _activated;

    QVariantList lst;
    foreach(BindPossibility bp, _possibilities)
        lst.append(bp.toVariantMap());
    ret["possibilities"] = lst;

    return ret;
}
