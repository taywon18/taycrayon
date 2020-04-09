#include "formfiller.h"
#include "ui_formfiller.h"
#include "utils.h"

#include <QMessageBox>
#include <QTextStream>
#include <QTextEdit>
#include <QLineEdit>
#include <QClipboard>
#include <QMenu>
#include <QFileDialog>
#include <QDateTime>

#include <QtDebug>
#include <QKeyEvent>

FormFiller::FormFiller(CharacterCollection* chars, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFiller)
{
    ui->setupUi(this);

    _chars = chars;

    QString actsPath = Utils::GetSampDirectoryPath() + "/acts";
    QDir d;
    d.mkpath(actsPath);

    _fsmodel = new QFileSystemModel();
    _fsmodel->setRootPath(actsPath);
    _fsmodel->setReadOnly(true);


    ui->actExplorerTV->setModel(_fsmodel);
    ui->actExplorerTV->setRootIndex(_fsmodel->index(actsPath));
    for (int i = 1; i < _fsmodel->columnCount(); ++i)
        ui->actExplorerTV->hideColumn(i);
    ui->actExplorerTV->expandAll();


    connect(ui->actExplorerTV, &QAbstractItemView::clicked, this, &FormFiller::onItemClicked);
    connect(ui->OtherGenerationTB, &QAbstractButton::clicked, this, &FormFiller::defaultGeneration);

    setFormFillerEnabled(false);
    reloadChars();

    QMenu* menu = new QMenu(this);
    menu->addAction("Générer dans le presse papier", this, &FormFiller::generateToClipboard);
    menu->addAction("Générer dans un fichier", [&](){ generateToFile(); });
    ui->OtherGenerationTB->setMenu(menu);
    ui->OtherGenerationTB->setPopupMode(QToolButton::MenuButtonPopup);
}

FormFiller::~FormFiller()
{
    delete ui;
}

void FormFiller::setFormFillerEnabled(bool enabled)
{
    ui->FormsTW->setEnabled(enabled);
    if(!enabled)
    {
        ui->FormsTW->clearContents();
    }
}

void FormFiller::select(QFileInfo fi)
{
    if(!fi.exists()){
        _current = QFileInfo();
        setFormFillerEnabled(true);
        return;
    }


    setFormFillerEnabled(true);
    _current = fi;

    _document.loadFromPath(fi.filePath());

    auto fields = _document.fields();


    int val = ui->CharacterCB->currentIndex();
    Character* c = nullptr;
    if(val >= 0)
        c = &(*_chars)[val];

    GenerationEnvironment env(fields, *c);


    ui->FormsTW->setColumnCount(2);
    ui->FormsTW->setRowCount(fields.count());

    QStringList headers;
    headers << "Clé" << "Valeur";
    ui->FormsTW->setHorizontalHeaderLabels(headers);
    ui->FormsTW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->FormsTW->horizontalHeader()->show();

    QMap<QString, QString>::iterator i;
    int row = 0;
    for (i = fields.begin(); i != fields.end(); ++i)
    {
        QString key = i.key();
        QStringList formats = i.value().toLower().split(",", QString::SkipEmptyParts);
        QString value = env.parse(i.key(), i.value());

        ui->FormsTW->setItem(row, 0, new QTableWidgetItem( key, Qt::DisplayRole));
        //ui->FormsTW->setItem(row, 1, new QTableWidgetItem( value, Qt::EditRole));



        bool important = formats.contains("important");
        if(important){
            QFont f = ui->FormsTW->font();
            f.setBold(true);
            ui->FormsTW->item(row, 0)->setFont(f);
        }

        if(formats.contains("multiline"))
        {
            QTextEdit *edit = new QTextEdit(this);
            edit->setText(value);
            ui->FormsTW->setRowHeight(row, 100);
            ui->FormsTW->setCellWidget(row, 1, edit);

            if(important){
                QPalette p = edit->palette();
                p.setColor(QPalette::Base, Qt::red);
                edit->setPalette(p);

                connect(edit, &QTextEdit::textChanged, [edit](){
                    QPalette p = edit->palette(); // define pallete for textEdit..
                    if(edit->toPlainText().isEmpty()){
                        p.setColor(QPalette::Base, Qt::red);
                    }
                    else {
                        p.setColor(QPalette::Base, Qt::white);
                    }
                    edit->setPalette(p);
                });
            }
        }
        else
        {
            QLineEdit *edit = new QLineEdit(this);
            edit->setText(value);
            ui->FormsTW->setRowHeight(row, 20);
            ui->FormsTW->setCellWidget(row, 1, edit);

            if(important){
                QPalette p = edit->palette();
                p.setColor(QPalette::Base, Qt::red);
                edit->setPalette(p);

                connect(edit, &QLineEdit::textChanged, [edit](){
                    QPalette p = edit->palette(); // define pallete for textEdit..
                    if(edit->text().isEmpty()){
                        p.setColor(QPalette::Base, Qt::red);
                    }
                    else {
                        p.setColor(QPalette::Base, Qt::white);
                    }
                    edit->setPalette(p);
                });
            }
        }

        row++;
    }
}

void FormFiller::reloadChars()
{
    ui->CharacterCB->clear();
    for(int i = 0; i < _chars->length(); i++)
    {
        QString readableStr = (*_chars)[i].toString();
        ui->CharacterCB->addItem(readableStr, QVariant(i));
    }
}

QString FormFiller::generate()
{
    QMap<QString, QString> values;

    for(int r = 0; r < ui->FormsTW->rowCount(); r++)
    {
        QString key = ui->FormsTW->item(r, 0)->text();
        QWidget* w = ui->FormsTW->cellWidget(r, 1);
        if (QLineEdit *le = qobject_cast<QLineEdit *>(w))
            values[key] = le->text();
        else if(QTextEdit *te = qobject_cast<QTextEdit *>(w))
            values[key] = te->toPlainText();
    }

    int val = ui->CharacterCB->currentIndex();
    Character* c = nullptr;
    if(val >= 0)
        c = &(*_chars)[val];

    FieldCollection fc;
    GenerationEnvironment env(fc, *c);
    return _document.generate(env, values);
}

void FormFiller::generateToClipboard()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(generate());
}

void FormFiller::generateToFile(QString path)
{
    QString defaultDir = Utils::GetSampDirectoryPath() + "/documents";
    QDir dir;
    dir.mkpath(defaultDir);
    QString defaultName = _current.fileName().split(".txt").first() + "-" + QDateTime::currentDateTime().toString("yyyymmdd-hhmmss") + ".txt";

    if(path.isEmpty())
    {
        path = QFileDialog::getSaveFileName(this,
                                     tr("Sauvegarder vers..."),
                                     defaultDir + "/" + defaultName
                                     );
    }

    if(path.isEmpty())
        return;

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QMessageBox::critical(this, "Impoosible de sauvegarder", QString(tr("Impossible d'ouvrir le fichier %1 en écriture.")).arg(path));
        return;
    }
    QTextStream out(&file);
    out << generate();
}

void FormFiller::defaultGeneration()
{
    QString defaultDir = Utils::GetSampDirectoryPath() + "/documents";
    QString defaultName = _current.fileName().split(".txt").first() + "-" + QDateTime::currentDateTime().toString("yyyymmdd-hhmmss") + ".txt";

    generateToFile(defaultDir + "/" + defaultName);
    generateToClipboard();

    qDebug() << _document.execCalls();
}

void FormFiller::onItemClicked(const QModelIndex &index)
{
	select(_fsmodel->fileInfo(index));
}

void FormFiller::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape)
		this->close();
}

