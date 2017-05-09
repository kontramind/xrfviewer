QT          += widgets quick
CONFIG      += c++11
TEMPLATE     = app
TARGET       = xrfviewer


INCLUDEPATH += ../xrfdump \
               C:/dev/dcmtk/install/include \

LIBS += -lwsock32 -ladvapi32 -lnetapi32 \
        -LC:/dev/dcmtk/ext/support/zlib/lib -lzlib_d \
        -LC:/dev/dcmtk/install/lib -lofstd -loflog -ldcmdata -ldcmimgle \

SOURCES += main.cpp \
    xrfimageprovider.cpp \
    ../xrfdump/xrfcineloop.cpp \
    ../xrfdump/xrfdcmutils.cpp \
    xrfcineloopmanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    xrfimageprovider.h \
    ../xrfdump/xrfcineloop.h \
    ../xrfdump/xrfdcmdeftag.h \
    xrfcineloopmanager.h
