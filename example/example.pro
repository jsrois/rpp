TEMPLATE = app
DEPENDPATH += . 
INCLUDEPATH += .

unix:CONFIG += debug_and_release

CONFIG += console
RESOURCES += generator.qrc
CONFIG += release

win32: CONFIG += console
mac:CONFIG -= app_bundle

include(../src/rxx.pri)
include(../src/rpp/rpp.pri)

SOURCES += main.cpp

QT = core
