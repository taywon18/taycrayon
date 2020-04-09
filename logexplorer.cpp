#include "logexplorer.h"
#include "ui_logexplorer.h"

LogExplorer::LogExplorer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogExplorer)
{
    ui->setupUi(this);
}

LogExplorer::~LogExplorer()
{
    delete ui;
}
