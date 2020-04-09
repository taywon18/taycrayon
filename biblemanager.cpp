#include "biblemanager.h"
#include "ui_biblemanager.h"

BibleManager::BibleManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BibleManager)
{
    ui->setupUi(this);
}

BibleManager::~BibleManager()
{
    delete ui;
}
