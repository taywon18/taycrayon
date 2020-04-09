#ifndef CHARACTERSETTINGS_H
#define CHARACTERSETTINGS_H

#include <QWidget>

namespace Ui {
class CharacterSettings;
}

class CharacterSettings : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterSettings(QWidget *parent = nullptr);
    ~CharacterSettings();

private:
    Ui::CharacterSettings *ui;
};

#endif // CHARACTERSETTINGS_H
