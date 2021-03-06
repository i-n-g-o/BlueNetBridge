QT += quick
QT += bluetooth

CONFIG += c++11
#CONFIG += i386

osx: INCLUDEPATH += /usr/include
osx: QMAKE_LFLAGS += -F/System/Library/Frameworks -L/usr/lib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    devicecontroller.cpp \
    appcontroller.cpp \
    bluefruitleserialdevice.cpp \
    messagereceiver.cpp \
    bleserialdevice.cpp \
    bledummybledevice.cpp

HEADERS += \
    devicecontroller.h \
    appcontroller.h \
    bluefruitleserialdevice.h \
    messagereceiver.h \
    bleserialdevice.h \
    bledummybledevice.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    android/AndroidManifest.xml \
#    android/gradle/wrapper/gradle-wrapper.jar \
#    android/gradlew \
#    android/res/values/libs.xml \
#    android/build.gradle \
#    android/gradle/wrapper/gradle-wrapper.properties \
#    android/gradlew.bat

#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ios {
    setting.name = DEVELOPMENT_TEAM
    setting.value = VAPJ2M8U5L
    QMAKE_MAC_XCODE_SETTINGS += setting

#    QMAKE_DEVELOPMENT_TEAM = VAPJ2M8U5L
#    QMAKE_PROVISINOING_PROFILE = 27841dce-210b-45d4-b8a8-ca0e9611ec80
#    QMAKE_TARGET_BUNDLE_PREFIX = i.n.g.o.app
    QMAKE_INFO_PLIST = ios/Info.plist
}
