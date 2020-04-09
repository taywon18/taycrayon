#ifndef FORMFILLER_H
#define FORMFILLER_H

#include <QWidget>

namespace Ui {
class FormFiller;
}

class FormFiller : public QWidget
{
    Q_OBJECT

public:
    explicit FormFiller(QWidget *parent = nullptr);
    ~FormFiller();

private:
    Ui::FormFiller *ui;
};

#endif // FORMFILLER_H
