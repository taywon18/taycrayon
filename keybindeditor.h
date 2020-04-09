#ifndef KEYBINDEDITOR_H
#define KEYBINDEDITOR_H

#include <QWidget>

namespace Ui {
class KeybindEditor;
}

class KeybindEditor : public QWidget
{
    Q_OBJECT

public:
    explicit KeybindEditor(QWidget *parent = nullptr);
    ~KeybindEditor();

private:
    Ui::KeybindEditor *ui;
};

#endif // KEYBINDEDITOR_H
