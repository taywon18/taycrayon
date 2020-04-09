#include "keybindlist.h"
#include "ui_keybindlist.h"

KeybindList::KeybindList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeybindList)
{
    ui->setupUi(this);
}

KeybindList::~KeybindList()
{
    delete ui;
}
