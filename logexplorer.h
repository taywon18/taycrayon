#ifndef LOGEXPLORER_H
#define LOGEXPLORER_H

#include <QWidget>

namespace Ui {
class LogExplorer;
}

class LogExplorer : public QWidget
{
    Q_OBJECT

public:
    explicit LogExplorer(QWidget *parent = nullptr);
    ~LogExplorer();

private:
    Ui::LogExplorer *ui;
};

#endif // LOGEXPLORER_H
