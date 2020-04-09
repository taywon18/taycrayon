#ifndef KEYBINDEDITOR_H
#define KEYBINDEDITOR_H

#include <QWidget>
#include "bindcontainer.h"

namespace Ui {
class KeybindEditor;
}

class QComboBox;

class KeybindEditor : public QWidget
{
    Q_OBJECT

public:
    explicit KeybindEditor(QWidget *parent = nullptr);
    ~KeybindEditor();
    void setTarget(Bind* target);

public slots:
    void saveCurrentPossibility();
    void displayCustomContextMenu(const QPoint &pos);
    void currentIndexChange(int row);
    void keySequenceChanged(const QKeySequence& ks);


signals:
    void shouldSave();


private:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void setCondition(QComboBox* b, BindRequierments::Condition cond);
    void refresh();
    Ui::KeybindEditor *ui;
    Bind* _target = nullptr;
    int _currentPossibility = 0;
};

#endif // KEYBINDEDITOR_H
