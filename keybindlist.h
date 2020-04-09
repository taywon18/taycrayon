#ifndef KEYBINDLIST_H
#define KEYBINDLIST_H

#include <QWidget>
#include "bindcontainer.h"
#include "keybindeditor.h"

namespace Ui {
class KeybindList;
}

class KeybindList : public QWidget
{
    Q_OBJECT

public:
    explicit KeybindList(BindContainer& binds, QWidget *parent = nullptr);
    ~KeybindList();

public slots:
    void onContextMenu(const QPoint &point);
    void refreshContent();
signals:
    void shouldRefreshBinder();

private:
    Ui::KeybindList *ui;
    BindContainer& _binds;
    KeybindEditor* _editor;
    QList<QWidget*> _lastWidgets;
    void keyPressEvent(QKeyEvent *event);
};

#endif // KEYBINDLIST_H
