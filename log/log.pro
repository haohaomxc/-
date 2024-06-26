QT       += core gui
QT+=sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canttchartscene.cpp \
    canttchartview.cpp \
    cantttimebaritem.cpp \
    guanliwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    manager.cpp \
    putong.cpp \
    renwuwindow.cpp \
    show.cpp

HEADERS += \
    canttchartscene.h \
    canttchartview.h \
    cantttimebaritem.h \
    definition.h \
    guanliwindow.h \
    mainwindow.h \
    manager.h \
    putong.h \
    renwuwindow.h \
    show.h

FORMS += \
    guanliwindow.ui \
    mainwindow.ui \
    manager.ui \
    putong.ui \
    renwuwindow.ui \
    show.ui

TRANSLATIONS += \
    log_zh_CN.ts

QT += core gui sql
QT += network
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc \
    resources.qrc
