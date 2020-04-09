#include "samplinkdialog.h"
#include "ui_samplinkdialog.h"
#include "utils.h"

#include <QMessageBox>

#include <windows.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>

SampLinkDialog::SampLinkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SampLinkDialog)
{
    ui->setupUi(this);
	ui->nicknameLE->setHidden(true);
}

SampLinkDialog::~SampLinkDialog()
{
	delete ui;
}

bool SampLinkDialog::createShortcut(QString name, QString targetpath, QString args)
{

	HRESULT hres;
	IShellLink* psl;

	// Get a pointer to the IShellLink interface.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
							IID_IShellLink, (LPVOID*)&psl);
	if (!SUCCEEDED(hres))
		return false;

	IPersistFile* ppf;

	// Set the path to the shortcut target and add the description.
	psl->SetPath(targetpath.toStdWString().c_str());
	psl->SetDescription(name.toStdWString().c_str());
	psl->SetArguments(args.toStdWString().c_str() );

	// Query IShellLink for the IPersistFile interface for saving the
	// shortcut in persistent storage.
	hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

	if (!SUCCEEDED(hres))
	{
		psl->Release();
		return false;
	}

	QString pathOfLnk = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "\\" + name + ".lnk";
	// Save the link by calling IPersistFile::Save.
	hres = ppf->Save(pathOfLnk.toStdWString().c_str(), TRUE);
	if(!SUCCEEDED(hres))
	{
		psl->Release();
		return false;
	}

	ppf->Release();
	return true;
}

void SampLinkDialog::accept()
{
	QString name = ui->NameLE->text();
	QString address = ui->addressLE->text();
	QString nickname = ui->nicknameLE->text();


	if(name.isEmpty())
	{
		ui->NameLE->setFocus();
		return;
	}

	if(address.isEmpty())
	{
		ui->addressLE->setFocus();
		return;
	}

	/*if(nickname.isEmpty())
	{
		ui->nicknameLE->setFocus();
		return;
	}*/

	QString target, args;
	int choice = ui->startChoiceCB->currentIndex();
	if(choice == 2)
	{
		target = Utils::GetSampExecutablePath();
		//args = address + " -n " + nickname;
		args = address;
	}
	else if(choice == 1)
	{
		target =  QCoreApplication::applicationFilePath();
		//args = "-l \"" + address + " -n " + nickname + "\"";
		args = "-l \"" + address + "\"";
	}
	else if(choice == 0)
	{
		target =  QCoreApplication::applicationFilePath();
		//args = "-s -l \"" + address + " -n " + nickname + "\"";
		args = "-s -l \"" + address + "\"";
	}

	bool r = createShortcut(name, target, args);
	if(!r)
	{
		QMessageBox::critical(this, "Impossible de créer le raccourci", "La création du raccourci a échoué.");
		return;
	}

	QDialog::accept();
}
