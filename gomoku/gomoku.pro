QT += core gui
QT += core network
QT += multimedia
QT += concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    board.cpp \
    connect.cpp \
    main.cpp \
    mainwindow.cpp \
    offline.cpp \
    p2psocket.cpp \
    playerai.cpp \
    settings.cpp \
    online.cpp

HEADERS += \
    board.h \
    connect.h \
    mainwindow.h \
    offline.h \
    p2psocket.h \
    playerai.h \
    settings.h \
    online.h

FORMS += \
    connect.ui \
    mainwindow.ui \
    offline.ui \
    settings.ui \
    online.ui

RESOURCES += resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

