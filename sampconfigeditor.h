#ifndef SAMPCONFIGEDITOR_H
#define SAMPCONFIGEDITOR_H

#include <QWidget>

namespace Ui {
class SampConfigEditor;
}

class SampConfigEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SampConfigEditor(QWidget *parent = nullptr);
    ~SampConfigEditor();

private:
    Ui::SampConfigEditor *ui;
};

#endif // SAMPCONFIGEDITOR_H
