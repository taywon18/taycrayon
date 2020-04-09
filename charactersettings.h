#ifndef CHARACTERSETTINGS_H
#define CHARACTERSETTINGS_H

#include <QWidget>
#include "charactercollection.h"

namespace Ui {
class CharacterSettings;
}

class CharacterSettings : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterSettings(CharacterCollection* target, QWidget *parent = nullptr);
    ~CharacterSettings();

public slots:
    void load();
    void add();
    void select(int row);

private slots:
    void removeSelected();
    void setFieldsEnabled(bool enabled);
    void editFinished();
    void onSelected(int row);

signals:
    void shouldSave();

private:
    Ui::CharacterSettings *ui;
    CharacterCollection* _target;

    void keyPressEvent(QKeyEvent *event);
};

#endif // CHARACTERSETTINGS_H
