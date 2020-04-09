#include "samplinkdialog.h"
#include "ui_samplinkdialog.h"

SampLinkDialog::SampLinkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SampLinkDialog)
{
    ui->setupUi(this);
}

SampLinkDialog::~SampLinkDialog()
{
    delete ui;
}
