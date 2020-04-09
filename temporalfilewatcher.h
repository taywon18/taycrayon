#ifndef TEMPORALFILEWATCHER_H
#define TEMPORALFILEWATCHER_H

#include <QObject>

class TemporalFileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit TemporalFileWatcher(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TEMPORALFILEWATCHER_H
