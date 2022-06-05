QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base.cpp \
    base/Config.cpp \
    base/Ench.cpp \
    base/EnchSet.cpp \
    base/FlowStep.cpp \
    base/Item.cpp \
    base/ItemPool.cpp \
    base/Summary.cpp \
    base/EnchTable.cpp \
    base/WeaponTable.cpp \
    lists/EnchList.cpp \
    lists/FlowList.cpp \
    lists/ItemEditor.cpp \
    lists/ItemList.cpp \
    lists/WeaponBox.cpp \
    lists/WeaponList.cpp \
    main.cpp \
    AWindow.cpp \
    Settings.cpp \
    CheckUpdate.cpp \
    CalculatingThread.cpp \
    WaitingWidget.cpp \
    FastCustomizing.cpp \
    MsgBox.cpp \
    TableEditor.cpp \
    storage\DataManager.cpp \
    storage/FileOperator.cpp \
    storage/Logger.cpp

HEADERS += \
    base.h \
    AWindow.h \
    Settings.h \
    CheckUpdate.h \
    CalculatingThread.h \
    WaitingWidget.h \
    FastCustomizing.h \
    MsgBox.h \
    TableEditor.h \
    base/Config.h \
    base/Ench.h \
    base/EnchSet.h \
    base/FlowStep.h \
    base/Item.h \
    base/ItemPool.h \
    base/Summary.h \
    base/EnchTable.h \
    base/WeaponTable.h \
    lists/EnchListItemWidget.h \
    lists/EnchListWidget.h \
    lists/FlowListItemWidget.h \
    lists/FlowListWidget.h \
    lists/ItemEditor.h \
    lists/ItemListItemWidget.h \
    lists/ItemListWidget.h \
    lists/WeaponBox.h \
    lists/WeaponListItemWidget.h \
    lists/WeaponListWidget.h \
    storage\DataManager.h \
    storage/FileOperator.h \
    storage/Logger.h

FORMS += \
    AWindow.ui \
    lists/EnchListItemWidget.ui \
    lists/FlowListItemWidget.ui \
    lists/ItemEditor.ui \
    lists/ItemListItemWidget.ui \
    Settings.ui \
    TableEditor.ui \
    WaitingWidget.ui \
    lists/WeaponListItemWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc \
    lib.qrc

DISTFILES += \
    EnchantmentTable.csv \
    README.md \
    README.md \
    WeaponTable.csv \
    logo.rc \
    update.json

RC_FILE += logo.rc

TARGET = "BestEnchSeq_v3.0"
