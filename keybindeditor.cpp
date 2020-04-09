#include "keybindeditor.h"
#include "ui_keybindeditor.h"

KeybindEditor::KeybindEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeybindEditor)
{
    ui->setupUi(this);
}

KeybindEditor::~KeybindEditor()
{
    delete ui;
}
