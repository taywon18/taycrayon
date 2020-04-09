#ifndef QSINGLEAPPLICATION_H
#define QSINGLEAPPLICATION_H

#include <QApplication>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QThread>

class QSingleApplication
    : public QThread
{

    Q_OBJECT

    QApplication _app;

    bool _isMain;
    QSharedMemory _singular; // shared memory !! SINGLE ACCESS
    QSystemSemaphore _readWriteLocker;
    void run();
    void runAsClient();
    void runAsMaster();

signals:
    void signaled(QString text);

public:
    QSingleApplication(int &argc, char **argv);
    ~QSingleApplication();
    bool shouldLaunch();
    void launch();
    void sendToLaunched(QString message);
    int exec();
    QStringList arguments();
};

#endif // QSINGLEAPPLICATION_H
