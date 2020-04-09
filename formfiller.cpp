#include "formfiller.h"
#include "ui_formfiller.h"

FormFiller::FormFiller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFiller)
{
    ui->setupUi(this);
}

FormFiller::~FormFiller()
{
    delete ui;
}
