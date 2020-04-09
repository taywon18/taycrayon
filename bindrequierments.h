#ifndef BINDREQUIERMENTS_H
#define BINDREQUIERMENTS_H
#include <QVariant>

enum class PedStatus{
    UnknownStatus = 0x0,
    Ped = 0x1,
    Car = 0x2,
    Cycle = 0x4,
    Choppa = 0x8,
    Boat = 0x10
};

enum class ChatState{
    Close = 0,
    Open,
    OpenOrClose
};


inline PedStatus operator|(PedStatus lhs, PedStatus rhs)
{
    return static_cast<PedStatus>(static_cast<char>(lhs) | static_cast<char>(rhs));
}

inline PedStatus operator&(PedStatus lhs, PedStatus rhs)
{
    return static_cast<PedStatus>(static_cast<char>(lhs) & static_cast<char>(rhs));
}

struct BindEnvironment{
    PedStatus _status = PedStatus::UnknownStatus;
    bool chatOpen = true;
    int live = 100;
    int armor = 100;
    int vehLive = -1;
};

struct BindRequierments{
    enum Condition{
        Equal,
        Different,
        Less,
        LessOrEqual,
        Greater,
        GreaterOrEqual
    };

    bool liveNeeded = false;
    int liveAmount;
    Condition liveCondition;

    bool armorNeeded = false;
    int armorAmount;
    Condition armorCondition;

    bool vehLiveNeeded = false;
    int vehLiveAmount;
    Condition vehLiveCondition;

    PedStatus status = PedStatus::UnknownStatus;
    ChatState chat = ChatState::Close;

    BindRequierments(QVariant var = QVariant());
    QVariantMap toVariantMap();

    bool match(const BindEnvironment& env);
    bool match(int a, Condition c, int b);


public:
    bool getLiveNeeded() const;
    void setLiveNeeded(bool value);

    int getLiveAmount() const;
    void setLiveAmount(int value);

    Condition getLiveCondition() const;
    void setLiveCondition(const Condition &value);

    bool getArmorNeeded() const;
    void setArmorNeeded(bool value);

    int getArmorAmount() const;
    void setArmorAmount(int value);

    Condition getArmorCondition() const;
    void setArmorCondition(const Condition &value);

    bool getVehLiveNeeded() const;
    void setVehLiveNeeded(bool value);

    int getVehLiveAmount() const;
    void setVehLiveAmount(int value);

    Condition getVehLiveCondition() const;
    void setVehLiveCondition(const Condition &value);

    PedStatus getStatus() const;
    void setStatus(const PedStatus &value);

    ChatState getChat() const;
    void setChat(const ChatState &value);

private:
};

#endif // BINDREQUIERMENTS_H
