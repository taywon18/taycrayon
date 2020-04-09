#include "keybindlist.h"
#include "ui_keybindlist.h"

#include <QMenu>
#include <QKeySequenceEdit>
#include <QtDebug>
#include <QSettings>
#include <QCheckBox>
#include <QKeyEvent>

KeybindList::KeybindList(BindContainer& binds, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeybindList),
    _binds(binds)
{
    ui->setupUi(this);
    _editor = new KeybindEditor(nullptr);
    connect(_editor, &KeybindEditor::shouldSave, [this](){
        emit shouldRefreshBinder();
        this->refreshContent();
    });

    ui->keybindListTW->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->keybindListTW, &QTableWidget::customContextMenuRequested, this, &KeybindList::onContextMenu);

    refreshContent();
}

void KeybindList::refreshContent()
{
    ui->keybindListTW->clearContents();

    foreach(QWidget* w, _lastWidgets)
        delete w;
    _lastWidgets.clear();

    ui->keybindListTW->setRowCount(_binds.count());
    ui->keybindListTW->setColumnCount(3);


    QList<QWidget*> lastWidgets;
    for(int r = 0; r < _binds.count(); r++)
    {
        const Bind& b = _binds[r];

        QCheckBox* cb = new QCheckBox();
        lastWidgets.append(cb);
        cb->setChecked(b.activated());
        ui->keybindListTW->setCellWidget(r, 0, cb);

        QKeySequenceEdit* seqEdit = new QKeySequenceEdit();
        lastWidgets.append(seqEdit);
        seqEdit->setKeySequence(b.keySequence());
        ui->keybindListTW->setCellWidget(r, 1, seqEdit);

        QTableWidgetItem* le = new QTableWidgetItem(b.name());
        ui->keybindListTW->setItem(r, 2, le);
	}
}

void KeybindList::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape)
		close();
}

KeybindList::~KeybindList()
{
    delete ui;
    delete _editor;
}

void KeybindList::onContextMenu(const QPoint &point)
{
    qDebug() << point;
    QModelIndex index = ui->keybindListTW->indexAt(point);

    QMenu *menu=new QMenu(this);

    if (index.isValid()) {
        menu->addAction("Modifier", [this, index](){
            _editor->setTarget(&_binds[index.row()]);
            _editor->show();
        });

        menu->addAction("Supprimer", [this, index](){
            _binds.removeAt(index.row());
            emit shouldRefreshBinder();
        });
    }
    else
     {
        menu->addAction("Nouveau bind", [this](){
            Bind b;
            _binds.append(b);
            _editor->setTarget(&_binds.last());
            _editor->show();
        });
    }

    menu->popup(ui->keybindListTW->viewport()->mapToGlobal(point));

}
