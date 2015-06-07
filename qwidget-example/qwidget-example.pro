include(../qmapboxgl.pri)

TEMPLATE = app

TARGET = qwidget-example

QT += widgets

INCLUDEPATH += ../include

LIBS += -L../lib -lqmapboxgl

SOURCES += qwidget-example.cpp

HEADERS += ../include/qmapboxgl.h

RESOURCES += ../styles/styles.qrc

