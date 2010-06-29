TEMPLATE = lib
TARGET = QmlJSInspector
INCLUDEPATH += .
DEPENDPATH += .
QT += declarative network

include(../../private_headers.pri)

DEFINES += QMLJSINSPECTOR_LIBRARY

HEADERS += \
qmljsprivateapi.h \
qmljsdebuggerclient.h \
qmljsinspector_global.h \
qmljsinspectorconstants.h \
qmljsinspectorcontext.h \
qmljsinspectorplugin.h \
qmljsclientproxy.h \
qmljsinspector.h \
qmljsdelta.h

SOURCES += \
qmljsdebuggerclient.cpp \
qmljsinspectorcontext.cpp \
qmljsinspectorplugin.cpp \
qmljsclientproxy.cpp \
qmljsinspector.cpp \
qmljsdelta.cpp

OTHER_FILES += QmlJSInspector.pluginspec
RESOURCES += qmljsinspector.qrc

include(../../qtcreatorplugin.pri)
include(../../plugins/projectexplorer/projectexplorer.pri)
include(../../plugins/qmlprojectmanager/qmlprojectmanager.pri)
include(../../plugins/coreplugin/coreplugin.pri)
include(../../plugins/texteditor/texteditor.pri)
include(../../plugins/debugger/debugger.pri)
