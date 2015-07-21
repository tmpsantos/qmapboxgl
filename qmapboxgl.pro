# Note: remember to keep these versions up to date.
LIBUV_VERSION=0.10.28
SQLITE_VERSION=3.8.8.1

SUBDIRS = example

TEMPLATE = app

QT += opengl

CONFIG += link_pkgconfig debug

QMAKE_CXXFLAGS += -std=c++11

macx {
    !system(which mason >/dev/null) {
        MASON_URL=https://github.com/mapbox/mason installation
        error(Mason not found; Please follow the installation steps in $$join(MASON_URL, $$LITERAL_HASH))
    }

    message(Checking for libuv $$LIBUV_VERSION ...)
    !system(mason install libuv $$LIBUV_VERSION >/dev/null) {
        error(unable to install libuv $$LIBUV_VERSION - please check your internet connection)
    }

    message(Checking for sqlite $$SQLITE_VERSION ...)
    !system(mason install sqlite $$SQLITE_VERSION >/dev/null) {
        error(unable to install sqlite $$SQLITE_VERSION - please check your internet connection)
    }

    # Qt disables pkg-config by default on OSX.
    QT_CONFIG -= no-pkg-config

    # MapboxGL static libs depends on these specific dependency versions.
    LIBS += \
        $$system(mason ldflags libuv 0.10.28) \
        $$system(mason prefix libuv 0.10.28)/lib/libuv.a \
        $$system(mason ldflags sqlite 3.8.8.1) \
        $$system(mason prefix sqlite 3.8.8.1)/lib/libsqlite3.a \
        -framework Foundation \
        -framework ApplicationServices
}

PKGCONFIG += libmbgl

SOURCES += example/example.cpp \
           src/qmapboxgl.cpp

HEADERS += example/example.h \
           include/qmapboxgl.h \
           src/qmapboxgl_p.h

RESOURCES += styles/styles.qrc

INCLUDEPATH += include
