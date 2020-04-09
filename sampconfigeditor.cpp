#include "sampconfigeditor.h"
#include "ui_sampconfigeditor.h"

SampConfigEditor::SampConfigEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SampConfigEditor)
{
    ui->setupUi(this);
}

SampConfigEditor::~SampConfigEditor()
{
    delete ui;
}
