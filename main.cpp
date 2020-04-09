#include "mainwindow.h"
#include "qsingleapplication.h"
#include "gtakeybinder.h"
#include <QtDebug>
#include "utils.h"
#include <QProcess>
#include <QMetaObject>

int main(int argc, char *argv[])
{
	QSingleApplication a(argc, argv);

    QStringList args = a.arguments();
	if(int launcherPos = args.indexOf("-l"); launcherPos != -1 && launcherPos+1 < args.count())
	{
		QString cmd = "\"" + Utils::GetSampExecutablePath() + "\" " + args[launcherPos + 1].remove('"');

		if(!QProcess::startDetached(cmd))
			MessageBox(
			  NULL,
			  cmd.toStdWString().c_str(),
			  L"Impossible de lancer GTA",
			  MB_OK
			);
    }

    if(!a.shouldLaunch())
    {
        a.sendToLaunched("focus");
        return 0;
    }

    a.launch();


    GtaKeybinder::instance();


	QCoreApplication::setOrganizationName("Taywon Inc.");
	QCoreApplication::setOrganizationDomain("nowhere");
	QCoreApplication::setApplicationName("LSPDprime");

    MainWindow w;

    QObject::connect(&a, &QSingleApplication::signaled, [&](QString str){
        if(str == "focus")
            QMetaObject::invokeMethod(&w, "refocus", Qt::QueuedConnection);
    });

	if(!a.arguments().contains("-s", Qt::CaseInsensitive))
        w.show();

    return a.exec();
}
