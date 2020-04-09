#ifndef KEYBINDLIST_H
#define KEYBINDLIST_H

#include <QWidget>

namespace Ui {
class KeybindList;
}

class KeybindList : public QWidget
{
    Q_OBJECT

public:
    explicit KeybindList(QWidget *parent = nullptr);
    ~KeybindList();

private:
    Ui::KeybindList *ui;
};

#endif // KEYBINDLIST_H
