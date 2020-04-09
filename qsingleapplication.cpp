#include "qsingleapplication.h"
#include <QTextStream>
#include <QtDebug>


QSingleApplication::QSingleApplication(int &argc, char **argv)
    : _app(argc, argv, true)
    , _singular("taycrayon-com")
    , _readWriteLocker("taycrayon-rwlock", 1)
{
    _singular.setNativeKey("taycrayon-rwlock");
}

QSingleApplication::~QSingleApplication()
{
    terminate();
    if(_singular.isAttached())
            _singular.detach();
}

bool QSingleApplication::shouldLaunch()
{
    if(_singular.attach()) //si on peut s'y attacher, c'est que ça existe
    {
        _singular.detach();

        // vérifier l'état du processus
        _isMain = false;

        return false;
    }

    qDebug() << "QSingleApplication::shouldLaunch: " << _singular.errorString();

    return true;
}

void QSingleApplication::launch()
{
    if(!_singular.create(1024))
    {
        qDebug() << "QSingleApplication::launch: " << _singular.errorString();
    }

    _isMain = true;
    _readWriteLocker.acquire();
    start();
}

void QSingleApplication::sendToLaunched(QString message)
{
    if(!_singular.isAttached())
        if(!_singular.attach())
            qDebug() << "QSingleApplication::sendToLaunched: attached return" << _singular.errorString();

    _singular.lock();


    void* target = _singular.data();

    memcpy(target, (void*)message.toStdString().c_str(), message.length() * sizeof(char));

    _singular.unlock();
    _singular.detach();


    _readWriteLocker.release(); // send message !
    qDebug() << "send message: " << message;
}

int QSingleApplication::exec()
{
    return _app.exec();
}

QStringList QSingleApplication::arguments()
{
    return _app.arguments();
}

void QSingleApplication::run()
{
    if(_isMain)
        runAsMaster();
    else
        runAsClient();
}

void QSingleApplication::runAsClient()
{

}

void QSingleApplication::runAsMaster()
{
    while(true){
        _readWriteLocker.acquire();

        _singular.lock();
        char const * from = ( char const * )_singular.data();

        QString text;
        QTextStream stream( &text );
        while ( from && *from )
        {
            stream << *from;
            ++from;
        }
        _singular.unlock();

        emit signaled(text);
    }
}
