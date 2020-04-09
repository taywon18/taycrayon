#include "bindpossibility.h"

QString BindPossibility::content() const
{
    return _content;
}

void BindPossibility::setContent(const QString &content)
{
    _content = content;
}

bool BindPossibility::cleanChatBeforeBind() const
{
    return _cleanChatBeforeBind;
}

void BindPossibility::setCleanChatBeforeBind(bool cleanChatBeforeBind)
{
    _cleanChatBeforeBind = cleanChatBeforeBind;
}

bool BindPossibility::openChatEachBeginLine() const
{
    return _openChatEachBeginLine;
}

void BindPossibility::setOpenChatEachBeginLine(bool openChatEachBeginLine)
{
    _openChatEachBeginLine = openChatEachBeginLine;
}

bool BindPossibility::sendChatEachEndLine() const
{
    return _sendChatEachEndLine;
}

void BindPossibility::setSendChatEachEndLine(bool sendChatEachEndLine)
{
    _sendChatEachEndLine = sendChatEachEndLine;
}

bool BindPossibility::removeCapsLock() const
{
    return _removeCapsLock;
}

void BindPossibility::setRemoveCapsLock(bool removeCapsLock)
{
    _removeCapsLock = removeCapsLock;
}

bool BindPossibility::putCursorAtFirstPositionWhenStart() const
{
    return _putCursorAtFirstPositionWhenStart;
}

void BindPossibility::setPutCursorAtFirstPositionWhenStart(bool putCursorAtFirstPositionWhenStart)
{
    _putCursorAtFirstPositionWhenStart = putCursorAtFirstPositionWhenStart;
}

bool BindPossibility::putCursorAtLastPositionWhenEnd() const
{
    return _putCursorAtLastPositionWhenEnd;
}

void BindPossibility::setPutCursorAtLastPositionWhenEnd(bool putCursorAtLastPositionWhenEnd)
{
    _putCursorAtLastPositionWhenEnd = putCursorAtLastPositionWhenEnd;
}

BindRequierments& BindPossibility::requierments()
{
    return _requierments;
}

QString BindPossibility::name() const
{
    return _name;
}

void BindPossibility::setName(const QString &name)
{
    _name = name;
}

bool BindPossibility::doNotSendIfParameterMissing() const
{
    return _doNotSendIfParameterMissing;
}

void BindPossibility::setDoNotSendIfParameterMissing(bool doNotSendIfParameterMissing)
{
    _doNotSendIfParameterMissing = doNotSendIfParameterMissing;
}

BindPossibility::BindPossibility(QVariant var)
{    
    auto obj = var.toMap();
    
    _content = obj.value("content", _content).toString();
    _name = obj.value("name", "Par défaut").toString();

    _cleanChatBeforeBind = obj.value("cleanChatBeforeBind", _cleanChatBeforeBind).toBool();
    _openChatEachBeginLine = obj.value("openChatEachBeginLine", _openChatEachBeginLine).toBool();
    _sendChatEachEndLine = obj.value("sendChatEachEndLine", _sendChatEachEndLine).toBool();
    _removeCapsLock = obj.value("removeCapsLock", _removeCapsLock).toBool();
    _putCursorAtFirstPositionWhenStart = obj.value("putCursorAtFirstPositionWhenStart", _putCursorAtFirstPositionWhenStart).toBool();
    _putCursorAtLastPositionWhenEnd = obj.value("putCursorAtLastPositionWhenEnd", _putCursorAtLastPositionWhenEnd).toBool();
    _doNotSendIfParameterMissing = obj.value("doNotSendIfParameterMissing", _doNotSendIfParameterMissing).toBool();


    _requierments = BindRequierments(obj.value("requierment", QVariant()));
}

QVariantMap BindPossibility::toVariantMap()
{
    QVariantMap ret;

    ret["content"] = _content;
    ret["name"] = _name;
    ret["cleanChatBeforeBind"] = _cleanChatBeforeBind;
    ret["openChatEachBeginLine"] = _openChatEachBeginLine;
    ret["sendChatEachEndLine"] = _sendChatEachEndLine;
    ret["removeCapsLock"] = _removeCapsLock;
    ret["putCursorAtFirstPositionWhenStart"] = _putCursorAtFirstPositionWhenStart;
    ret["putCursorAtLastPositionWhenEnd"] = _putCursorAtLastPositionWhenEnd;
    ret["doNotSendIfParameterMissing"] = _doNotSendIfParameterMissing;
    ret["requierments"] = _requierments.toVariantMap();

    return ret;
}

