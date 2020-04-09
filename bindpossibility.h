#ifndef BINDPOSSIBILITY_H
#define BINDPOSSIBILITY_H
#include <QString>
#include <QVariant>

#include "bindrequierments.h"

class BindPossibility
{
    QString _content, _name;
    bool _cleanChatBeforeBind = true;
    bool _openChatEachBeginLine = true;
    bool _sendChatEachEndLine = true;
    bool _removeCapsLock = true;
    bool _doNotSendIfParameterMissing = true;
    bool _putCursorAtFirstPositionWhenStart = false;
    bool _putCursorAtLastPositionWhenEnd = false;

    BindRequierments _requierments;

public:
    BindPossibility(QVariant var = QVariant());

    QVariantMap toVariantMap();
    QString content() const;
    void setContent(const QString &content);
    bool cleanChatBeforeBind() const;
    void setCleanChatBeforeBind(bool cleanChatBeforeBind);
    bool openChatEachBeginLine() const;
    void setOpenChatEachBeginLine(bool openChatEachBeginLine);
    bool sendChatEachEndLine() const;
    void setSendChatEachEndLine(bool sendChatEachEndLine);
    bool removeCapsLock() const;
    void setRemoveCapsLock(bool removeCapsLock);
    bool putCursorAtFirstPositionWhenStart() const;
    void setPutCursorAtFirstPositionWhenStart(bool putCursorAtFirstPositionWhenStart);
    bool putCursorAtLastPositionWhenEnd() const;
    void setPutCursorAtLastPositionWhenEnd(bool putCursorAtLastPositionWhenEnd);

    BindRequierments& requierments();
    QString name() const;
    void setName(const QString &name);
    bool doNotSendIfParameterMissing() const;
    void setDoNotSendIfParameterMissing(bool doNotSendIfParameterMissing);
};

#endif // BINDPOSSIBILITY_H
