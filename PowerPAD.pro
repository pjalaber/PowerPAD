POWERPAD_VERSION_STR = 0.1.1
POWERPAD_VERSION = 0x000101
POWERPAD_VERSION_WIN32 = 0,1,1,0
POWERPAD_COPYRIGHT = "(c) 2020 Tekit - Philippe Jalaber"

DEFINES += POWERPAD_VERSION_STR=$$POWERPAD_VERSION_STR \
           POWERPAD_VERSION=$$POWERPAD_VERSION \
           POWERPAD_VERSION_WIN32=$$POWERPAD_VERSION_WIN32 \
           POWERPAD_COPYRIGHT=\"\\\"$$POWERPAD_COPYRIGHT\\\"\" \
           POWERPAD_BUILD_DATE=$$system(powershell "(date -UFormat %s).split(',')[0]") \
           POWERPAD_BUILD_REVISION=$$system(git rev-parse --short=10 HEAD)

QT += quick widgets quickcontrols2 svg xml

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
        singleappinstance.cpp \
        update.cpp \
        winsys.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    PowerPAD_fr_FR.ts

# Use our own manifest file
CONFIG -= embed_manifest_exe
RC_FILE = PowerPAD.rc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

QMAKE_CXXFLAGS += -Wall -Werror

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
    singleappinstance.h \
    update.h \
    winsys.h

specified_configs=$$find(CONFIG, "\b(debug|release)\b")
BUILD_SUBDIR=$$last(specified_configs)
BINARY_CREATOR=$$[QT_INSTALL_PREFIX]/../../Tools/QtInstallerFramework/3.2.3/bin/binarycreator.exe
REPOGEN=$$[QT_INSTALL_PREFIX]/../../Tools/QtInstallerFramework/3.2.3/bin/repogen.exe
LUPDATE=$$[QT_INSTALL_BINS]/lupdate.exe
LRELEASE=$$[QT_INSTALL_BINS]/lrelease.exe
DOLLAR = $
CHANGELOG=$$cat(changelog.html)
CHANGELOG=$$replace(CHANGELOG, "<", "&lt;")
CHANGELOG=$$replace(CHANGELOG, "<", "&gt;")
CHANGELOG=$$replace(CHANGELOG, "'", "&apos;")
CHANGELOG=$$replace(CHANGELOG, "\"", "\\\"")


installer.target = installer
installer.commands = \
    (if exist \"$${OUT_PWD}/$${BUILD_SUBDIR}/installer\" (rd /S/Q \"$${OUT_PWD}/$${BUILD_SUBDIR}/installer\")) && \
    md \"$${OUT_PWD}/$${BUILD_SUBDIR}/installer\" && \
    xcopy \"$${PWD}/installer\" \"$${OUT_PWD}/$${BUILD_SUBDIR}/installer\" /E/Y && \
    copy /Y $$shell_path(\"$${OUT_PWD}/$${BUILD_SUBDIR}/PowerPAD.exe\") $$shell_path(\"$${OUT_PWD}/$${BUILD_SUBDIR}/installer/packages/com.tekit.powerpad/data\") && \
    (PowerShell -Command \"Remove-Item \'$${OUT_PWD}/$${BUILD_SUBDIR}/installer/config/*.ts\' \") && \
    (PowerShell -Command \"Remove-Item \'$${OUT_PWD}/$${BUILD_SUBDIR}/installer/packages/com.tekit.powerpad/meta/*.ts\' \") && \
    PowerShell -Command \"Get-ChildItem \'$${OUT_PWD}/$${BUILD_SUBDIR}/installer\' -Recurse -File -Include *.xml | \
    ForEach { \
        (Get-Content $${DOLLAR}$${DOLLAR}_).Replace(\'|APP_CHANGELOG|\', \'$${CHANGELOG}\').Replace(\'|APP_VERSION|\', \'$${POWERPAD_VERSION_STR}\').Replace(\'|RELEASE_DATE|\', (Date -Format \'yyyy-MM-dd\')) | \
        Set-Content $${DOLLAR}$${DOLLAR}_ \
    }\" && \
    (if exist \"$${OUT_PWD}/$${BUILD_SUBDIR}/repo\" (rd /S/Q \"$${OUT_PWD}/$${BUILD_SUBDIR}/repo\")) && \
    \"$${REPOGEN}\" -v -p \"$${OUT_PWD}/$${BUILD_SUBDIR}/installer/packages\" -i com.tekit.powerpad \"$${OUT_PWD}/$${BUILD_SUBDIR}/repo\" && \
    \"$${BINARY_CREATOR}\" --online-only -v -e com.tekit.powerpad -c \"$${OUT_PWD}/$${BUILD_SUBDIR}/installer/config/config.xml\" -p \"$${OUT_PWD}/$${BUILD_SUBDIR}/installer/packages\" \"$${OUT_PWD}/$${BUILD_SUBDIR}/repo/PowerPADSetup64\" && \
    \"$${BINARY_CREATOR}\" --offline-only -v -c \"$${OUT_PWD}/$${BUILD_SUBDIR}/installer/config/config.xml\" -p \"$${OUT_PWD}/$${BUILD_SUBDIR}/installer/packages\" \"$${OUT_PWD}/$${BUILD_SUBDIR}/repo/PowerPADOfflineSetup64\"

lupdate.target = lupdate
lupdate.commands = \
$${LUPDATE} -no-recursive \"$${PWD}\" -ts \"$${PWD}/PowerPAD_fr_FR.ts\" && \
$${LUPDATE} -no-recursive \"$${PWD}/installer/packages/com.tekit.powerpad/meta\" -ts \"$${PWD}/installer/packages/com.tekit.powerpad/meta/fr.ts\" && \
$${LUPDATE} -no-recursive \"$${PWD}/installer/config\" -ts \"$${PWD}/installer/config/fr.ts\"

lrelease.target = lrelease
lrelease.commands = \
$${LRELEASE} \"$${PWD}/PowerPAD_fr_FR.ts\" -qm \"$${PWD}/languages/PowerPAD_fr_FR.qm\" && \
$${LRELEASE} \"$${PWD}/installer/packages/com.tekit.powerpad/meta/fr.ts\" -qm \"$${PWD}/installer/packages/com.tekit.powerpad/meta/fr.qm\" && \
$${LRELEASE} \"$${PWD}/installer/config/fr.ts\" -qm \"$${PWD}/installer/config/fr.qm\"

QMAKE_EXTRA_TARGETS += installer lupdate lrelease

DISTFILES += \
    PowerPAD.manifest \
    PowerPAD.rc \
    changelog.html \
    installer/config/config.xml \
    installer/config/controller.qs \
    installer/config/fr.ts \
    installer/config/icon.png \
    installer/packages/com.tekit.powerpad/meta/fr.ts \
    installer/packages/com.tekit.powerpad/meta/gplv3.txt \
    installer/packages/com.tekit.powerpad/meta/installscript.qs \
    installer/packages/com.tekit.powerpad/meta/runprogram.ui \
    installer/packages/com.tekit.powerpad/meta/package.xml
