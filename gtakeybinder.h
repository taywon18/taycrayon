#ifndef GTAPROCESSMODIFIER_H
#define GTAPROCESSMODIFIER_H

#include <QObject>
#include <QThread>
#include <windows.h>

#include <QHash>
#include <QPair>
#include <QString>

#include "neighboorfinder.h"
#include "bindcontainer.h"


class GtaKeybinder
        : QObject
{
    struct SpecialBindOptions{
        SpecialBindOptions() = default;
        SpecialBindOptions(BindPossibility& bp);

        bool CleanChatBeforeBind = false;
        bool OpenChatEachBeginLine = false;
        bool SendChatEachEndLine = false;
        bool RemoveCapsLock = false;
        bool PutCursorAtBeginBeforeBind = false;
        bool PutCursorAtEndAfterBind = false;
    };

    Q_OBJECT
    HHOOK _hook;
    ~GtaKeybinder();
    static GtaKeybinder* _instance;
    explicit GtaKeybinder(QObject *parent = nullptr);

public:
    void keybind(Bind b);
    void directKeybind(const QString& str);
    void directKeybind(const QString& str, SpecialBindOptions sbo);


    bool onKey(int nCode, WPARAM wParam, LPARAM lParam);
    bool match(QKeySequence seq, int nCode, bool ctrlpressed, bool majpressed = false, bool altpressed = false);


	HANDLE getOpenedGtaProcess(bool foregroundOnly = true);
    BindEnvironment getBindEnvironment(HANDLE gtaProcess);



    static GtaKeybinder& instance();

    void setBinds(BindContainer *binds);

signals:

public slots:

private:    
    void inject();
    template<typename F> F distance(F ax, F ay, F az, F bx, F by, F bz);
    template<typename F> F distanceSq(F ax, F ay, F az, F bx, F by, F bz);
    void keybind(QList<BindPossibility> bp, HANDLE gtaProcess = nullptr);
    bool keybind(BindPossibility bp, HANDLE gtaProcess = nullptr);
    QList<QPair<QString, QString>> replace(QSet<QString> keywords, HANDLE gtaProcess, bool* missing = nullptr);
    HWND GetSampForegroundWindows();
	HWND GetSampWindows();
    NeighboorFinder _finder;
    BindContainer* _binds = nullptr;
    bool _active = true;
    bool _chatOpen = false;
};

#endif // GTAPROCESSMODIFIER_H
