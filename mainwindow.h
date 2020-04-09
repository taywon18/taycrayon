#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "charactersettings.h"
#include "charactercollection.h"
#include "biblemanager.h"
#include "formfiller.h"
#include "logexplorer.h"
#include "keybindlist.h"
#include "bindcontainer.h"
#include "samplinkdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    FormFiller *filler() const;

public slots:
    void openCharacterSettings();
    void openBible();
    void openFiller();
    void openLog();
    void openBinds();
	void openLinks();
    void openAbout();
    void openAboutQt();

    void saveCharacters();
    void saveBinds();
	void startLastAddress();

public slots:
    void systrayIconActivated(QSystemTrayIcon::ActivationReason r);
    void refocus();

private:
    void createConnections();
    Ui::MainWindow *ui;
    CharacterSettings* _charsettings;
    BibleManager* _bible;
    CharacterCollection _characters;
    FormFiller* _filler;
    LogExplorer* _log;
    QSystemTrayIcon _trayIcon;
    KeybindList* _bindsettings;
    BindContainer _binds;

};

#endif // MAINWINDOW_H
