VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_PATCH = 0

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}
QMAKE_TARGET_COMPANY = "Tekit"
QMAKE_TARGET_PRODUCT = "PowerPAD"
QMAKE_TARGET_DESCRIPTION = "PowerPAD application"
QMAKE_TARGET_COPYRIGHT = "(c) 2020 Tekit - Philippe Jalaber"

DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\" \
           APP_COMPANY=\"\\\"$${QMAKE_TARGET_COMPANY}\\\"\" \
           APP_PRODUCT=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\" \
           APP_DESCRIPTION=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\" \
           APP_COPYRIGHT=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\" \
           APP_BUILD_DATE=\"\\\"$$system(powershell "(date -UFormat %s).split(',')[0]")\\\"\" \
           APP_BUILD_REVISION=\"\\\"$$system(git rev-parse --short=10 HEAD)\\\"\"

QT += quick widgets quickcontrols2

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        action.cpp \
        button.cpp \
        controller.cpp \
        helper.cpp \
        keyboard.cpp \
        main.cpp \
        mouseacceleration.cpp \
        settings.cpp \
        winsys.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    PowerPAD_fr_FR.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

QMAKE_CXXFLAGS += -Werror

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    action.h \
    button.h \
    controller.h \
    helper.h \
    keyboard.h \
    mouseacceleration.h \
    settings.h \
    winsys.h

DISTFILES +=
