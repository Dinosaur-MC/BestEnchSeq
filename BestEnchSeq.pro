QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    best_ench_seq.cpp \
    common.cpp \
    enchantmentlist.cpp \
    main.cpp \
    selector.cpp

HEADERS += \
    best_ench_seq.h \
    common.h \
    enchantmentlist.h \
    selector.h

FORMS += \
    best_ench_seq.ui \
    selector.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc

DISTFILES += \
    LICENSE \
    README.md \
    logo.rc

RC_FILE += logo.rc
