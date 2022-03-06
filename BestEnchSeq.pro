QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basic.cpp \
    calculator.cpp \
    checkupdate.cpp \
    enchlist.cpp \
    enchlistwidget.cpp \
    feedback.cpp \
    fileoperate.cpp \
    flowlist.cpp \
    flowlistwidget.cpp \
    itemeditor.cpp \
    itemlist.cpp \
    itemlistwidget.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    settings.cpp \
    sort.cpp \
    tableeditor.cpp \
    waitwidget.cpp

HEADERS += \
    basic.h \
    calculator.h \
    checkupdate.h \
    enchlist.h \
    enchlistwidget.h \
    feedback.h \
    fileoperate.h \
    flowlist.h \
    flowlistwidget.h \
    itemeditor.h \
    itemlist.h \
    itemlistwidget.h \
    logger.h \
    mainwindow.h \
    settings.h \
    sort.h \
    tableeditor.h \
    waitwidget.h

FORMS += \
    enchlistwidget.ui \
    flowlistwidget.ui \
    itemeditor.ui \
    itemlistwidget.ui \
    mainwindow.ui \
    settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc

DISTFILES += \
    EnchantmentTable.txt \
    LICENSE \
    README.md \
    logo.rc

RC_FILE += logo.rc
