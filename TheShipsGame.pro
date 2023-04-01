QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AbstractShip.cpp \
    NPCShip.cpp \
    UsersShips.cpp \
    main.cpp \
    mainwindow.cpp \
    spawnpoint.cpp

HEADERS += \
    AbstractShip.h \
    NPCShip.h \
    UsersShip.h \
    mainwindow.h \
    shipsvector.h \
    spawnpoint.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    recource.qrc

DISTFILES +=
