#ifndef TEMPORALFILEWATCHER_H
#define TEMPORALFILEWATCHER_H

#include <QObject>
#include <QFileSystemWatcher>
#include "bindcontainer.h"

class TemporalFileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit TemporalFileWatcher(QObject *parent = nullptr);

signals:
    void newLine(QString line);

public slots:
    void setActive(bool active);

private slots:
    void onFileChanged(const QString& path = QString());

private:
    QFileSystemWatcher* _watcher = nullptr;
};

#endif // TEMPORALFILEWATCHER_H
