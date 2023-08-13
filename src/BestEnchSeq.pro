QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
#CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithms/algenumerate.cpp \
    algorithms/alggreedy.cpp \
    algorithms/alghuffmantree.cpp \
    core/algmanager.cpp \
    core/algorithm.cpp \
    core/calculator.cpp \
    core/core.cpp \
    core/fileoperator.cpp \
    core/interactor.cpp \
    core/tablemanager.cpp \
    launcher.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/console.cpp \
    ui/graphics.cpp \
    updatechecker.cpp

HEADERS += \
    algorithms/algenumerate.h \
    algorithms/alggreedy.h \
    algorithms/alghuffmantree.h \
    core/algmanager.h \
    core/algorithm.h \
    core/calculator.h \
    core/core.h \
    core/fileoperator.h \
    core/interactor.h \
    core/tablemanager.h \
    launcher.h \
    mainwindow.h \
    ui/console.h \
    ui/graphics.h \
    updatechecker.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    langs/zh_cn.ts \
    langs/zh_tw.ts \
    langs/lzh_cn.ts \
    langs/en_us.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    langs/langs.qrc \
    res/res.qrc \
    lib/lib.qrc

DISTFILES += \
    README.md \
    lang/Chinese.ts \
    res/tables/EnchantmentTable.csv \
    res/tables/WeaponTable.csv \
    update.json \
    logo.rc

VERSION = "4.2.0"
TARGET = "BestEnchSeq"
RC_FILE += logo.rc
