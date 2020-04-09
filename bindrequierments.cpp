#include "bindrequierments.h"
#include <QDebug>

bool BindRequierments::getLiveNeeded() const
{
    return liveNeeded;
}

void BindRequierments::setLiveNeeded(bool value)
{
    liveNeeded = value;
}

int BindRequierments::getLiveAmount() const
{
    return liveAmount;
}

void BindRequierments::setLiveAmount(int value)
{
    liveAmount = value;
}

BindRequierments::Condition BindRequierments::getLiveCondition() const
{
    return liveCondition;
}

void BindRequierments::setLiveCondition(const Condition &value)
{
    liveCondition = value;
}

bool BindRequierments::getArmorNeeded() const
{
    return armorNeeded;
}

void BindRequierments::setArmorNeeded(bool value)
{
    armorNeeded = value;
}

int BindRequierments::getArmorAmount() const
{
    return armorAmount;
}

void BindRequierments::setArmorAmount(int value)
{
    armorAmount = value;
}

BindRequierments::Condition BindRequierments::getArmorCondition() const
{
    return armorCondition;
}

void BindRequierments::setArmorCondition(const Condition &value)
{
    armorCondition = value;
}

bool BindRequierments::getVehLiveNeeded() const
{
    return vehLiveNeeded;
}

void BindRequierments::setVehLiveNeeded(bool value)
{
    vehLiveNeeded = value;
}

int BindRequierments::getVehLiveAmount() const
{
    return vehLiveAmount;
}

void BindRequierments::setVehLiveAmount(int value)
{
    vehLiveAmount = value;
}

BindRequierments::Condition BindRequierments::getVehLiveCondition() const
{
    return vehLiveCondition;
}

void BindRequierments::setVehLiveCondition(const Condition &value)
{
    vehLiveCondition = value;
}

PedStatus BindRequierments::getStatus() const
{
    return status;
}

void BindRequierments::setStatus(const PedStatus &value)
{
    status = value;
}

ChatState BindRequierments::getChat() const
{
    return chat;
}

void BindRequierments::setChat(const ChatState &value)
{
    chat = value;
}

BindRequierments::BindRequierments(QVariant var)
{
    auto obj = var.toMap();
    if(obj.empty())
        return;
    
    liveNeeded = obj.value("liveNeeded", liveNeeded).toBool();
    liveAmount = obj.value("liveAmount", liveAmount).toInt();
    liveCondition = static_cast<Condition>(obj.value("liveCondition", liveCondition).toInt());

    armorNeeded = obj.value("armorNeeded", armorNeeded).toBool();
    armorAmount = obj.value("armorAmount", armorAmount).toInt();
    armorCondition = static_cast<Condition>(obj.value("armorCondition", armorCondition).toInt());

    vehLiveNeeded = obj.value("vehLiveNeeded", vehLiveNeeded).toBool();
    vehLiveAmount = obj.value("vehLiveAmount", vehLiveAmount).toInt();
    vehLiveCondition = static_cast<Condition>(obj.value("vehLiveCondition", vehLiveCondition).toInt());

    status = static_cast<PedStatus>(obj.value("status", static_cast<int>(status)).toInt());
    chat = static_cast<ChatState>(obj.value("chat", static_cast<int>(chat)).toInt());
}

QVariantMap BindRequierments::toVariantMap()
{
    QVariantMap ret;
    ret["liveNeeded"] = liveNeeded;
    ret["liveAmount"] = liveAmount;
    ret["liveCondition"] = static_cast<int>(liveCondition);

    ret["armorNeeded"] = armorNeeded;
    ret["armorAmount"] = armorAmount;
    ret["armorCondition"] = static_cast<int>(armorCondition);

    ret["vehLiveNeeded"] = vehLiveNeeded;
    ret["vehLiveAmount"] = vehLiveAmount;
    ret["vehLiveCondition"] = static_cast<int>(vehLiveCondition);

    ret["status"] = static_cast<int>(status);
    ret["chat"] = static_cast<int>(chat);

    return ret;
}

bool BindRequierments::match(const BindEnvironment &env)
{
    if(liveNeeded)
        if(!match(env.live, liveCondition, liveAmount))
            return false;

    if(armorNeeded)
        if(!match(env.armor, armorCondition, armorAmount))
            return false;

    if(vehLiveNeeded)
        if(!match(env.vehLive, vehLiveCondition, vehLiveAmount))
            return false;

    qDebug() << "chatstatus" << env.chatOpen << static_cast<int>(chat);
    if(chat == ChatState::Close && env.chatOpen)
        return false;
    if(chat == ChatState::Open && !env.chatOpen)
        return false;

    qDebug() << "wut ?";
    return true;
}

bool BindRequierments::match(int a, BindRequierments::Condition c, int b)
{
    if(a < 0 || b < 0)
        return true;

    if(c == Condition::Equal)
        return a == b;
    else if(c == Condition::Different)
        return a != b;
    else if(c == Condition::Less)
        return a < b;
    else if(c == Condition::LessOrEqual)
        return a <= b;
    else if(c == Condition::Greater)
        return a > b;
    else if(c == Condition::GreaterOrEqual)
        return a >= b;

    return false;
}
