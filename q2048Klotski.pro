QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    2048-ai/2048ai.cpp \
    2048/2048.cpp \
    base/base.cpp \
    klotski/klotski.cpp \
    main.cpp \
    mainwindow/mainwindow.cpp \
    2048/window2048.cpp \
    klotski/windowklotski.cpp

HEADERS += \
    2048-ai/2048ai.h \
    2048-ai/config.h \
    2048/2048.h \
    base/base.h \
    klotski/klotski.h \
    mainwindow/mainwindow.h \
    2048-ai/platdefs.h \
    2048/window2048.h \
    klotski/windowklotski.h

FORMS += \
    mainwindow/mainwindow.ui \
    2048/window2048.ui \
    klotski/windowklotski.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
