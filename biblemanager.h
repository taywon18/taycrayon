#ifndef BIBLEMANAGER_H
#define BIBLEMANAGER_H

#include <QWidget>

namespace Ui {
class BibleManager;
}

class BibleManager : public QWidget
{
    Q_OBJECT

public:
    explicit BibleManager(QWidget *parent = nullptr);
    ~BibleManager();

private:
    Ui::BibleManager *ui;
};

#endif // BIBLEMANAGER_H
