QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basicdata.cpp \
    basiclists.cpp \
    basicoperator.cpp \
    basicvariable.cpp \
    checkupdate.cpp \
    main.cpp \
    awindow.cpp \
    settings.cpp \
    waitingwidget.cpp

HEADERS += \
    basicdata.h \
    awindow.h \
    basiclists.h \
    basicoperator.h \
    basicvariable.h \
    checkupdate.h \
    settings.h \
    waitingwidget.h

FORMS += \
    lists/itemwidget_ench.ui \
    lists/itemwidget_item.ui \
    lists/itemwidget_flowstep.ui \
    lists/itemeditor.ui \
    lists/weaponlistitemwidget.ui \
    awindow.ui \
    settings.ui \
    tableeditor.ui \
    waitingwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc \
    lib.qrc

DISTFILES += \
    README.md \
    README_EN.md \
    tables/EnchantmentTable.csv \
    tables/WeaponTable.csv \
    update.json \
    logo.rc

RC_FILE += logo.rc

TARGET = "BestEnchSeq_v4.0"
