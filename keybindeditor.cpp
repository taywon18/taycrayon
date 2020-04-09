#include "keybindeditor.h"
#include "ui_keybindeditor.h"

#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>

KeybindEditor::KeybindEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeybindEditor)
{
    ui->setupUi(this);

    ui->possibilitiesLW->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->possibilitiesLW, &QWidget::customContextMenuRequested, this, &KeybindEditor::displayCustomContextMenu);
    connect(ui->possibilitiesLW, &QListWidget::currentRowChanged, this, &KeybindEditor::currentIndexChange);
    connect(ui->keySequenceEdit, &QKeySequenceEdit::keySequenceChanged, this, &KeybindEditor::keySequenceChanged);
}

KeybindEditor::~KeybindEditor()
{
    delete ui;
}

void KeybindEditor::setTarget(Bind *target)
{
    _target = target;
    _currentPossibility = 0;
    refresh();
}

void KeybindEditor::saveCurrentPossibility()
{
    if(_target == nullptr)
        return;

    _target->setName( ui->descriptionLE->text() );
    _target->setKeySequence( ui->keySequenceEdit->keySequence() );

    auto& curpos = _target->possibilities()[_currentPossibility];

    curpos.setContent(ui->ContentTE->document()->toPlainText());
    curpos.setCleanChatBeforeBind(ui->cleanBeforeCB->isChecked());
    curpos.setOpenChatEachBeginLine(ui->openChatAtLineBeginLE->isChecked());
    curpos.setSendChatEachEndLine(ui->sendAtEndlCB->isChecked());
    curpos.setRemoveCapsLock(ui->noCapsLockCB->isChecked());
    curpos.setPutCursorAtLastPositionWhenEnd(ui->SeekAtEndCB->isChecked());
    curpos.setPutCursorAtFirstPositionWhenStart(ui->seekAtBeginCB->isChecked());
    curpos.setDoNotSendIfParameterMissing(ui->notSendIfMissingParameter->isChecked());

    curpos.requierments().liveNeeded = ui->isLiveConditionEnabled->isChecked();
    curpos.requierments().liveAmount = ui->LiveAmount->value();
    curpos.requierments().liveCondition = static_cast<BindRequierments::Condition>(ui->LiveCondition->currentIndex());

    curpos.requierments().armorNeeded = ui->isArmorConditionEnabled->isChecked();
    curpos.requierments().armorAmount = ui->ArmorAmount->value();
    curpos.requierments().armorCondition = static_cast<BindRequierments::Condition>(ui->ArmorCondition->currentIndex());

    curpos.requierments().vehLiveNeeded = ui->isCarLiveConditionEnabled->isChecked();
    curpos.requierments().vehLiveAmount = ui->LiveCarAmount->value();
    curpos.requierments().vehLiveCondition = static_cast<BindRequierments::Condition>(ui->CarLiveCondition->currentIndex());

    PedStatus status = PedStatus::UnknownStatus;
    if(ui->mayBeUsedWhenPed->isChecked()) status = status | PedStatus::Ped;
    if(ui->mayBeUsedInCar->isChecked()) status = status | PedStatus::Car;
    if(ui->mayBeUsedInBoat->isChecked()) status = status | PedStatus::Boat;
    if(ui->mayBeUsedInCycle->isChecked()) status = status | PedStatus::Cycle;
    if(ui->mayBeUsedInChoppa->isChecked()) status = status | PedStatus::Choppa;
    curpos.requierments().status = status;

    curpos.requierments().setChat(static_cast<ChatState>(ui->whenUsableCB->currentIndex()));

    emit shouldSave();
}

void KeybindEditor::displayCustomContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->possibilitiesLW->indexAt(pos);

    QMenu *menu=new QMenu(this);

    if (index.isValid()) {
        int r = index.row();

        menu->addAction("Supprimer", [&](){
            if(_target->possibilities().length() == 1)
            {
                QMessageBox::warning(this, "Impossible de supprimer la dernière possibilité", "Il doit rester au moins un possibilité.");
                return;
            }

            saveCurrentPossibility();
            _target->possibilities().removeAt(index.row());
            refresh();
        });

        menu->addAction("Renommer", [&](){
            QString oldname = _target->possibilities().at(r).name();
            bool ok;
            QString text = QInputDialog::getText(this, tr("Renommer la possibilité"),
                                                 tr("Nom de la possibilité: "), QLineEdit::Normal,
                                                 oldname, &ok);
            if (!ok || text.isEmpty())
                return;

            saveCurrentPossibility();
            _target->possibilities()[r].setName(text);
            refresh();
        });
    }
    else
     {
        menu->addAction("Nouvelle possibilité", [&](){
            bool ok;
            QString text = QInputDialog::getText(this, tr("Nommer la possibilité"),
                                                 tr("Nom de la possibilité: "), QLineEdit::Normal,
                                                 "", &ok);
            if (!ok || text.isEmpty())
                return;

            BindPossibility bp;
            bp.setName(text);

            saveCurrentPossibility();
            _target->possibilities().append(bp);
            _currentPossibility = _target->possibilities().length();
            refresh();
        });
    }

    menu->popup(ui->possibilitiesLW->viewport()->mapToGlobal(pos));
}

void KeybindEditor::currentIndexChange(int row)
{
    if(!_target)
        return;

    saveCurrentPossibility();
    if(row == _currentPossibility)
        return;

    _currentPossibility = row;
    refresh();
}

void KeybindEditor::keySequenceChanged(const QKeySequence &ks)
{
    if(ks.count() <= 1)
        return;

    QKeySequence ksc = QKeySequence(ks[ks.count()-1]);

    ui->keySequenceEdit->setKeySequence(ksc);
}

void KeybindEditor::closeEvent(QCloseEvent *event)
{
    saveCurrentPossibility();
}

void KeybindEditor::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        close();
}

void KeybindEditor::setCondition(QComboBox *b, BindRequierments::Condition cond)
{
    if(cond == BindRequierments::Equal)
        b->setCurrentIndex(0);
    else if(cond == BindRequierments::Different)
        b->setCurrentIndex(1);
    else if(cond == BindRequierments::Less)
        b->setCurrentIndex(2);
    else if(cond == BindRequierments::LessOrEqual)
        b->setCurrentIndex(3);
    else if(cond == BindRequierments::Greater)
        b->setCurrentIndex(4);
    else if(cond == BindRequierments::GreaterOrEqual)
        b->setCurrentIndex(5);
}

void KeybindEditor::refresh()
{
    if(_target == nullptr)
        return;
    auto target = _target;
    _target = nullptr;

    ui->descriptionLE->setText(target->name());
    ui->keySequenceEdit->setKeySequence(target->keySequence());

    auto allpos = target->possibilities();
    if(_currentPossibility >= allpos.count())
        _currentPossibility = 0;
    auto& curpos = allpos[_currentPossibility];

    ui->possibilitiesLW->clear();
    foreach(auto& p, allpos)
        ui->possibilitiesLW->addItem(new QListWidgetItem(p.name(), ui->possibilitiesLW));
    ui->possibilitiesLW->setItemSelected(ui->possibilitiesLW->item(_currentPossibility), true);

    ui->ContentTE->setPlainText(curpos.content());
    ui->cleanBeforeCB->setChecked(curpos.cleanChatBeforeBind());
    ui->openChatAtLineBeginLE->setChecked(curpos.openChatEachBeginLine());
    ui->sendAtEndlCB->setChecked(curpos.sendChatEachEndLine());
    ui->notSendIfMissingParameter->setChecked(curpos.doNotSendIfParameterMissing());
    ui->noCapsLockCB->setChecked(curpos.removeCapsLock());
    ui->seekAtBeginCB->setChecked(curpos.putCursorAtFirstPositionWhenStart());
    ui->SeekAtEndCB->setChecked(curpos.putCursorAtLastPositionWhenEnd());

    ui->isLiveConditionEnabled->setChecked( curpos.requierments().liveNeeded );
    ui->LiveAmount->setValue( curpos.requierments().liveAmount);
    setCondition(ui->LiveCondition, curpos.requierments().liveCondition);

    ui->isArmorConditionEnabled->setChecked( curpos.requierments().armorNeeded );
    ui->ArmorAmount->setValue( curpos.requierments().armorAmount);
    setCondition(ui->ArmorCondition, curpos.requierments().armorCondition);

    ui->isCarLiveConditionEnabled->setChecked( curpos.requierments().vehLiveNeeded );
    ui->LiveCarAmount->setValue( curpos.requierments().vehLiveAmount);
    setCondition(ui->CarLiveCondition, curpos.requierments().vehLiveCondition);

    auto status = curpos.requierments().status;
    ui->mayBeUsedWhenPed->setChecked(bool(status & PedStatus::Ped));
    ui->mayBeUsedInCar->setChecked(bool(status & PedStatus::Car));
    ui->mayBeUsedInBoat->setChecked(bool(status & PedStatus::Boat));
    ui->mayBeUsedInCycle->setChecked(bool(status & PedStatus::Cycle));
    ui->mayBeUsedInChoppa->setChecked(bool(status & PedStatus::Choppa));

    ui->whenUsableCB->setCurrentIndex(static_cast<int>(curpos.requierments().chat));

    _target = target;
}

