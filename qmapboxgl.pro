SUBDIRS = example

QT += widgets

TEMPLATE = app

CONFIG += link_pkgconfig opengl c++11 debug

PKGCONFIG += libmbgl

SOURCES += example/example.cpp \
           src/qmapboxgl.cpp

HEADERS += include/qmapboxgl.h

RESOURCES += styles/styles.qrc

INCLUDEPATH += include
