#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStandardPaths>
#include <QSettings>

namespace Utils
{
    inline QString GetSampDirectoryPath(){
        return QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + "/GTA San Andreas User Files/SAMP";
    }

    inline QString GetSampExecutablePath(){
        QString pathToSamp;
        QSettings config("HKEY_CLASSES_ROOT\\samp\\shell\\open\\command", QSettings::NativeFormat);
        if(config.status() != QSettings::NoError)
            return "";

        QString pathcod = config.value("Default").toString();
        if(pathcod.isEmpty())
            return "";
        QStringList pathcodlst = pathcod.split('\"');
        return pathcodlst.at(1);
    }

	inline QString GetConfigPath(){
		return GetSampDirectoryPath() + "/taycrayon.ini";
	}
}

#endif // UTILS_H
