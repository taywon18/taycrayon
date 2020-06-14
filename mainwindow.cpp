#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QIcon>
#include <QProcess>

#include "gtakeybinder.h"
#include "utils.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _trayIcon.setIcon(QIcon(":/sharpener.png"));
    connect(&_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::systrayIconActivated);
    _trayIcon.show();

	QSettings settings(Utils::GetConfigPath(), QSettings::IniFormat);
    _characters = CharacterCollection::FromVariant(settings.value("characters"));
    _binds = BindContainer(settings.value("binds"));
    GtaKeybinder::instance().setBinds(&_binds);

     _charsettings = new CharacterSettings(&_characters, nullptr);
    _filler = new FormFiller(&_characters, nullptr);
    _bible = new BibleManager(_filler, nullptr);
    _log = new LogExplorer(nullptr);
    _bindsettings = new KeybindList(_binds, nullptr);
	_feeder = new FeederControl(this);


    createConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _charsettings;
    delete _bible;
}

void MainWindow::createConnections()
{
    connect(ui->characterSetButton, &QAbstractButton::clicked, this, &MainWindow::openCharacterSettings);
    connect(ui->BibleButton, &QAbstractButton::clicked, this, &MainWindow::openBible);
    connect(ui->FillerButton, &QAbstractButton::clicked, this, &MainWindow::openFiller);
    connect(ui->LogButton, &QAbstractButton::clicked, this, &MainWindow::openLog);
    connect(ui->BindButton, &QAbstractButton::clicked, this, &MainWindow::openBinds);
	connect(ui->ShortcutButton, &QAbstractButton::clicked, this, &MainWindow::openLinks);
	connect(ui->ReconnectButton, &QAbstractButton::clicked, this, &MainWindow::startLastAddress);
	connect(ui->FeederButton, &QAbstractButton::clicked, this, &MainWindow::openFeeder);


    connect(_charsettings, &CharacterSettings::shouldSave, _filler, &FormFiller::reloadChars);
    connect(_charsettings, &CharacterSettings::shouldSave, this, &MainWindow::saveCharacters);
    connect(_bindsettings, &KeybindList::shouldRefreshBinder, this, &MainWindow::saveBinds);
    connect(ui->TayformerAction, &QAction::triggered, this, &MainWindow::openAbout);
    connect(ui->qtAction, &QAction::triggered, this, &MainWindow::openAboutQt);
}

FormFiller *MainWindow::filler() const
{
    return _filler;
}

void MainWindow::openCharacterSettings()
{
    _charsettings->show();
}

void MainWindow::openBible()
{
    _bible->show();
}

void MainWindow::openFiller()
{
	_filler->show();
}

void MainWindow::openFeeder()
{
	_feeder->show();
}

void MainWindow::openLog()
{
    _log->show();
}

void MainWindow::openBinds()
{
	_bindsettings->show();
}

void MainWindow::openLinks()
{
	SampLinkDialog smd(this);
	smd.exec();
}


void MainWindow::saveCharacters()
{
	QSettings settings(Utils::GetConfigPath(), QSettings::IniFormat);
    settings.setValue("characters", _characters.ToVariant());
}

void MainWindow::saveBinds()
{
	QSettings settings(Utils::GetConfigPath(), QSettings::IniFormat);
	settings.setValue("binds", _binds.toQVariantList());
}

void MainWindow::startLastAddress()
{
	QSettings set(Utils::GetConfigPath(), QSettings::IniFormat);
	QString address = set.value("lastAddress", "").toString();
	if(address.isEmpty())
	{
		QMessageBox::information(this, "Aucun serveur en mémoire", "Pour fonctionner, il faut que la gestion des logs soit activé et qu'il y ai eu connexion à un serveur.");
		return;
	}

	if(!QProcess::startDetached("\"" + Utils::GetSampExecutablePath() + "\" " + address)){
		QMessageBox::critical(this, "Impossible de lancer SAMP", QString("Path:") + Utils::GetSampExecutablePath() + " " + address);
	}
}

void MainWindow::openAbout()
{
	QMessageBox::about(this, "A propos du TayCrayon",
					   tr("Programmé avec amour et souplesse par la Goldenberg Philanthropy & Charity Foundation.\n")
					   + tr("Icone de livre: https://fr.m.wikipedia.org/wiki/Fichier:Closed_Book_Icon.svg")
					   + tr("Icone des logs: https://www.flaticon.com/free-icon/library_234648"));
}

void MainWindow::openAboutQt()
{
    QMessageBox::aboutQt(this, "A propos de Qt");
}

void MainWindow::systrayIconActivated(QSystemTrayIcon::ActivationReason r)
{
    if(r == QSystemTrayIcon::DoubleClick)
        this->show();
    else if(r == QSystemTrayIcon::Context)
    {
        QMenu m;
        m.addAction(tr("Fermer"), [](){
            QApplication::exit(0);
        });

        m.exec(QCursor::pos());
    }
}

void MainWindow::refocus()
{
    show();
    setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    raise();  // for MacOS
    activateWindow(); // for Windows
    setFocus();
}
