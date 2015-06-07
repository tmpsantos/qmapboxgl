include(../qmapboxgl.pri)

TEMPLATE = lib

CONFIG += static

TARGET = qmapboxgl

QT += widgets

INCLUDEPATH += ../include

HEADERS += ../include/qmapboxgl.h qmapboxgl_p.h

SOURCES += qmapboxgl.cpp

