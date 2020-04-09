#include "biblemanager.h"
#include "ui_biblemanager.h"
#include "mainwindow.h"


#include <QDir>
#include <QtDebug>
#include <QMessageBox>
#include <QMenu>
#include "utils.h"
#include "biblehighlighter.h"


BibleManager::BibleManager(FormFiller* ff, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BibleManager)
{
    _ff = ff;

    ui->setupUi(this);

    _highlighter = new BibleHighlighter(ui->contentLE->document());

    QString actsPath = Utils::GetSampDirectoryPath() + "/acts";
    QDir d;
    d.mkpath(actsPath);

    _fsmodel = new QFileSystemModel();
    _fsmodel->setRootPath(actsPath);
    _fsmodel->setReadOnly(false);

    ui->ActChoiceTV->setModel(_fsmodel);
    ui->ActChoiceTV->setRootIndex(_fsmodel->index(actsPath));
    for (int i = 1; i < _fsmodel->columnCount(); ++i)
        ui->ActChoiceTV->hideColumn(i);

    ui->ActChoiceTV->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ActChoiceTV->setDragEnabled(true);
    ui->ActChoiceTV->viewport()->setAcceptDrops(true);
    ui->ActChoiceTV->setDropIndicatorShown(true);
    ui->ActChoiceTV->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->ActChoiceTV->setDragDropMode(QAbstractItemView::InternalMove);


    connect(ui->ActChoiceTV, &QAbstractItemView::clicked, this, &BibleManager::onItemClicked);
    connect(ui->contentLE, &QTextEdit::textChanged, this, &BibleManager::contentChanged);
    connect(ui->ActChoiceTV, &QAbstractItemView::customContextMenuRequested, this, &BibleManager::onCustomContextMenuTv);

    connect(ui->saveButton, &QAbstractButton::clicked, this, &BibleManager::saveCurrent);
    connect(ui->reverseButton, &QAbstractButton::clicked, this, &BibleManager::reverse);
    connect(ui->generateButton, &QAbstractButton::clicked, this, &BibleManager::openInFormFiller);
    select();
}

BibleManager::~BibleManager()
{
    delete ui;
}

void BibleManager::setFieldEnabled(bool enabled)
{
    ui->contentLE->setEnabled(enabled);
    ui->generateButton->setEnabled(enabled);
    ui->reverseButton->setEnabled(enabled);
    ui->saveButton->setEnabled(enabled);

    if(!enabled)
    {
        ui->contentLE->clear();
    }

}

void BibleManager::onItemClicked(const QModelIndex &index)
{
    select(_fsmodel->fileInfo(index));    
}

void BibleManager::contentChanged()
{
    if(!current.exists())
        return;

    setChangePending(true);
}

void BibleManager::saveCurrent()
{
    if(!current.exists())
        return;

    QFile f(current.filePath());
    if(! f.open(QFile::WriteOnly  | QFile::Truncate | QFile::Text))
    {
        QMessageBox::warning(this, tr("Erreur !"), "Il n'a pas été possible d'ouvrir le fichier "+current.filePath()+".");
        select();
    }

    QTextStream out(&f);
    out << ui->contentLE->toPlainText();
    f.close();

    setChangePending(false);
}

void BibleManager::reverse()
{
    setChangePending(false);
    select(current);
}

void BibleManager::select(QFileInfo fi)
{
    if(_changesPending)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Le document a été modifié."));
        msgBox.setInformativeText("Voulez-vous sauvegarder les changements ?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();

        switch (ret) {
            case QMessageBox::Save:
                saveCurrent();
                break;
            case QMessageBox::Discard:
                reverse();
                break;
            case QMessageBox::Cancel:
            default:

                return;
            }


    }

    current = fi;

    if(!fi.exists() || !fi.isFile() || fi.isDir()) //strange, but needed...
    {
        current = QFileInfo();
        setFieldEnabled(false);
        return;
    }

    setFieldEnabled(true);

    QFile f(fi.filePath());
    if(! f.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Erreur !", "Il n'a pas été possible d'ouvrir le fichier "+fi.filePath()+".");
        select();
    }
    QTextStream in(&f);
    QString content = in.readAll();
    f.close();

    ui->contentLE->setPlainText(content);

    setChangePending(false);
}

void BibleManager::onCustomContextMenuTv(const QPoint &point)
{
    QModelIndex index = ui->ActChoiceTV->indexAt(point);

    QMenu *menu=new QMenu(this);

    if (index.isValid()) {
        QFileInfo fi = _fsmodel->fileInfo(index);
		menu->addAction("Supprimer", [fi, this](){
            QFile f(fi.filePath());
            f.remove();
			select();
        });
    }
    else
     {
        menu->addAction("Nouveau fichier", [](){
            int i = 1;
            while(true){
                QFile f(Utils::GetSampDirectoryPath() + "/acts/nouveau-" + QString::number( i ) +".txt");
                if(!f.exists() && f.open(QFile::WriteOnly | QFile::Text))
                    return;

                i++;
            }
        });

        menu->addAction("Nouveau dossier", [](){
            int i = 1;
            while(true){
                QDir d(Utils::GetSampDirectoryPath() + "/acts");
                QString dirname = "Nouveau-" + QString::number(i);
                if(!d.exists(dirname) && d.mkdir(dirname))
                    return;

                i++;
            }
        });
    }

    menu->popup(ui->ActChoiceTV->viewport()->mapToGlobal(point));

}

void BibleManager::setChangePending(bool bold)
{
    _changesPending = bold;
    QFont font = ui->saveButton->font();
    font.setBold(bold);
    ui->saveButton->setFont(font);
}

void BibleManager::openInFormFiller()
{
    if(!_ff)
        return;

    _ff->select(current);
    _ff->show();
    _ff->raise();
    _ff->setFocus();
}

void BibleManager::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        this->close();
}

