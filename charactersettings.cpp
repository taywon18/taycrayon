#include "charactersettings.h"
#include "ui_charactersettings.h"

#include <QMessageBox>
#include <QtDebug>
#include <QKeyEvent>

CharacterSettings::CharacterSettings(CharacterCollection* target, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharacterSettings),
    _target(target)
{
    ui->setupUi(this);

    setFieldsEnabled(false);

    connect(ui->charList, &QListWidget::currentRowChanged, this, &CharacterSettings::onSelected);
    connect(ui->AddCharacterButton, &QAbstractButton::clicked, this, &CharacterSettings::add);
    connect(ui->DeleteCharacterButton, &QAbstractButton::clicked, this, &CharacterSettings::removeSelected);

    connect(ui->mainFirstnameLE, &QLineEdit::editingFinished, this, &CharacterSettings::editFinished);
    connect(ui->otherFirstnamesLE, &QLineEdit::editingFinished, this, &CharacterSettings::editFinished);
    connect(ui->lastnameLE, &QLineEdit::editingFinished, this, &CharacterSettings::editFinished);
    connect(ui->professionLE, &QLineEdit::editingFinished, this, &CharacterSettings::editFinished);
    connect(ui->matricleLE, &QLineEdit::editingFinished, this, &CharacterSettings::editFinished);
    connect(ui->officeLE, &QLineEdit::editingFinished, this, &CharacterSettings::editFinished);

    load();
}

CharacterSettings::~CharacterSettings()
{
    delete ui;
}

void CharacterSettings::load()
{
    ui->charList->clear();
    for(auto c : *_target)
        ui->charList->addItem(c.firstNames().join(" ") + " " + c.lastName());
}

void CharacterSettings::add()
{
    editFinished();

    _target->append(Character());

    load();
    select(ui->charList->count() -1);

    emit shouldSave();
}

void CharacterSettings::select(int row)
{
    if(ui->charList->currentRow() != row)
        ui->charList->setCurrentRow(row);
}

void CharacterSettings::removeSelected()
{
    auto selected = ui->charList->currentRow();
    if(selected == -1)
    {
        QMessageBox::information(this, tr("Impossible"), tr("Veuillez selectionner un item à supprimer"));
        return;
    }

    QMessageBox msgBox;
    msgBox.setText(tr("Le choix courrant va être supprimé..."));
    msgBox.setInformativeText("Êtes-vous sur ?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Discard);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(ret != QMessageBox::Yes)
        return;

    _target->removeAt(selected);
    emit shouldSave();
    load();
}

void CharacterSettings::setFieldsEnabled(bool enabled)
{
    ui->mainFirstnameLE->setEnabled(enabled);
    ui->otherFirstnamesLE->setEnabled(enabled);
    ui->lastnameLE->setEnabled(enabled);
    ui->professionLE->setEnabled(enabled);
    ui->matricleLE->setEnabled(enabled);
    ui->officeLE->setEnabled(enabled);

    if(!enabled)
    {
        ui->mainFirstnameLE->clear();
        ui->otherFirstnamesLE->clear();
        ui->lastnameLE->clear();
        ui->professionLE->clear();
        ui->matricleLE->clear();
        ui->officeLE->clear();

    }
}

void CharacterSettings::editFinished()
{
    auto nselected = ui->charList->currentRow();
    if(nselected == -1)
        return;

    qDebug() << "CharacterSettings::editFinished :" << nselected;

    Character& selected = (*_target)[nselected];
    QStringList firstnames;
    QString mainfirstname = ui->mainFirstnameLE->text();
    QStringList otherfirstnames = ui->otherFirstnamesLE->text().split(" ", QString::SkipEmptyParts);
    if(mainfirstname.length())
        firstnames.append(mainfirstname);
    if(otherfirstnames.length())
        firstnames.append(otherfirstnames);
    selected.setFirstNames(firstnames);
    selected.setLastName(ui->lastnameLE->text());
    selected.setProfession(ui->professionLE->text());
    selected.setMatricle(ui->matricleLE->text());
    selected.setOffice(ui->officeLE->text());

    ui->charList->item(nselected)->setText(firstnames.join(" ") + " " + ui->lastnameLE->text());

    emit shouldSave();
}

void CharacterSettings::onSelected(int row)
{
    if(row < 0)
    {
        setFieldsEnabled(false);
        return;
    }
    setFieldsEnabled(true);

    auto curchar = _target->at(row);
    auto firstnames = curchar.firstNames();
    if(firstnames.length() >= 1)
    {
        ui->mainFirstnameLE->setText(firstnames.at(0));
        if(firstnames.length() >= 2)
            ui->otherFirstnamesLE->setText(firstnames.mid(1).join(" "));
        else
            ui->otherFirstnamesLE->setText(" ");
    }
    else {
        ui->mainFirstnameLE->setText("");
        ui->otherFirstnamesLE->setText("");
    }
    ui->lastnameLE->setText(curchar.lastName());
    ui->professionLE->setText(curchar.profession());
    ui->matricleLE->setText(curchar.matricle());
	ui->officeLE->setText(curchar.office());
}

void CharacterSettings::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape)
		this->close();
}

