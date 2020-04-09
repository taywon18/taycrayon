#include "charactersettings.h"
#include "ui_charactersettings.h"

CharacterSettings::CharacterSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharacterSettings)
{
    ui->setupUi(this);
}

CharacterSettings::~CharacterSettings()
{
    delete ui;
}
