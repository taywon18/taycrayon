#ifndef FORMFILLER_H
#define FORMFILLER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QFile>
#include "document.h"
#include "charactercollection.h"

namespace Ui {
class FormFiller;
}

class FormFiller : public QWidget
{
    Q_OBJECT

public:
    explicit FormFiller(CharacterCollection* charcol, QWidget *parent = nullptr);
    ~FormFiller();
    void setFormFillerEnabled(bool enabled);

public slots:
    void select(QFileInfo fi = QFileInfo());
    void reloadChars();
    QString generate();
    void generateToClipboard();

    void generateToFile(QString path = QString());
    void defaultGeneration();

private slots:
    void onItemClicked(const QModelIndex& index);

private:
    Ui::FormFiller *ui;
    CharacterCollection* _chars;
    QFileSystemModel* _fsmodel;
    QFileInfo _current;
    Document _document;
    void keyPressEvent(QKeyEvent *event);
};

#endif // FORMFILLER_H
