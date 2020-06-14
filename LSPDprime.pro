#-------------------------------------------------
#
# Project created by QtCreator 2020-01-19T14:53:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TayCrayon
TEMPLATE = app
RC_ICONS = res/sharpener.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        biblehighlighter.cpp \
        biblemanager.cpp \
        bind.cpp \
        bindcontainer.cpp \
        bindpossibility.cpp \
        bindrequierments.cpp \
        charactersettings.cpp \
        chathighlighter.cpp \
        document.cpp \
        feedercontrol.cpp \
        fieldcollection.cpp \
        formfiller.cpp \
        generationenvironment.cpp \
        gtainformation.cpp \
        gtakeybinder.cpp \
        keybindeditor.cpp \
        keybindlist.cpp \
        logexplorer.cpp \
        main.cpp \
        mainwindow.cpp \
        neighboorfinder.cpp \
        qsingleapplication.cpp \
        sampconfigeditor.cpp \
        samplinkdialog.cpp \
        settingsform.cpp \
        temporalfilewatcher.cpp

HEADERS += \
        biblehighlighter.h \
        biblemanager.h \
        bind.h \
        bindcontainer.h \
        bindpossibility.h \
        bindrequierments.h \
        character.h \
        charactercollection.h \
        charactersettings.h \
        chathighlighter.h \
        document.h \
        feedercontrol.h \
        field.h \
        fieldcollection.h \
        formfiller.h \
        generationenvironment.h \
        gtainformation.h \
        gtakeybinder.h \
        gtastruct.h \
        keybindeditor.h \
        keybindlist.h \
        keymap.h \
        logexplorer.h \
        mainwindow.h \
        neighboorfinder.h \
        qsingleapplication.h \
        sampconfigeditor.h \
        samplinkdialog.h \
        settingsform.h \
        temporalfilewatcher.h \
        utils.h

FORMS += \
        biblemanager.ui \
        charactersettings.ui \
        feedercontrol.ui \
        formfiller.ui \
        keybindeditor.ui \
        keybindlist.ui \
        logexplorer.ui \
        mainwindow.ui \
        sampconfigeditor.ui \
        samplinkdialog.ui \
        settingsform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    sources.txt

RESOURCES += \
    res/res.qrc

CONFIG += static

QMAKE_LFLAGS            += -static -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
DEFINES+=STATIC

QMAKE_LFLAGS += -l:libwinpthread.a
