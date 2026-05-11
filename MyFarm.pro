QT += widgets multimedia
CONFIG += c++11
CONFIG += resources_big

TEMPLATE = app
TARGET = MyFarm

SOURCES += \
    main.cpp \
    gamecanvaswidget.cpp \
    farmlandwidget.cpp \
    imagebutton.cpp \
    mainwindow.cpp \
    actiondialog.cpp

HEADERS += \
    gametypes.h \
    gamecanvaswidget.h \
    farmlandwidget.h \
    imagebutton.h \
    mainwindow.h \
    actiondialog.h

RESOURCES += \
    resources.qrc
