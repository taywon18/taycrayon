#include "temporalfilewatcher.h"
#include "utils.h"
#include <QDir>
#include <QDateTime>
#include <QtDebug>

TemporalFileWatcher::TemporalFileWatcher(QObject *parent)
    : QObject(parent)
{
    QDir dir;
    dir.mkpath(Utils::GetSampDirectoryPath() + "/chatlogs");

    _watcher = new QFileSystemWatcher();
    connect(_watcher, &QFileSystemWatcher::directoryChanged, this, &TemporalFileWatcher::onFileChanged);
}

void TemporalFileWatcher::setActive(bool active)
{
    if(!active)
        _watcher->removePath(Utils::GetSampDirectoryPath());
    else{
        _watcher->addPath(Utils::GetSampDirectoryPath());
        onFileChanged();
    }
}

void TemporalFileWatcher::onFileChanged(const QString & str)
{

    QString sampPath = Utils::GetSampDirectoryPath();
    QString originalPath = sampPath + "/chatlog.txt";
    QString renamedPath = sampPath + "/chatlog.work";

    QFileInfo chatlogfi(originalPath);
    QDateTime chatlogLastEditDay = chatlogfi.lastModified();

    QString daylog = sampPath + "/chatlogs/" + chatlogLastEditDay.toString("yyyyMMdd") + ".txt";


    QFile::remove(renamedPath);
    if(!QFile::rename(originalPath, renamedPath))
        return;

    QFile renamed(renamedPath);
    if(!renamed.open(QFile::ReadOnly | QFile::Text))
        return;

    QString newcontent = QTextStream(&renamed).readAll();

    QFile old(daylog);
    if(!old.open(QFile::WriteOnly | QFile::Text | QFile::Append))
        return;
    QTextStream stream( &old );
    for(QString line : newcontent.split('\n', QString::SkipEmptyParts)){
        stream << line << "\n";
        emit newLine(line);
    }
    renamed.remove();
}
