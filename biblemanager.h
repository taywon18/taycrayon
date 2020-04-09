#ifndef BIBLEMANAGER_H
#define BIBLEMANAGER_H

#include <QWidget>
#include <QFileSystemModel>

#include <QRegularExpression>
#include <QTextFormat>
#include "biblehighlighter.h"

class FormFiller;


namespace Ui {
class BibleManager;
}

class BibleManager : public QWidget
{
    Q_OBJECT

public:
    explicit BibleManager(FormFiller* mw, QWidget *parent = nullptr);
    ~BibleManager();

    void setFieldEnabled(bool enabled);

private slots:
    void onItemClicked(const QModelIndex& index);
    void contentChanged();
    void saveCurrent();
    void reverse();

public slots:
    void select(QFileInfo fi = QFileInfo());
    void onCustomContextMenuTv(const QPoint& point);
    void setChangePending(bool bold);
    void openInFormFiller();

private:
    bool _changesPending = false;
    Ui::BibleManager *ui;
    QFileSystemModel* _fsmodel;
    QFileInfo current;
    BibleHighlighter
    * _highlighter;
    FormFiller* _ff;

    void keyPressEvent(QKeyEvent *event);


};

#endif // BIBLEMANAGER_H
