# QMapboxGL

QMapboxGL is a Qt-enabled viewport for vector tiles using [Mapbox GL](https://github.com/mapbox/mapbox-gl-native) for rendering. It can be used for online and offline rendering of vector tiles.

### Demo

A simple example rendering the Mapbox base vector tiles layer based on [OpenStreetMap](http://www.openstreetmap.org) using different styles.

![](https://i.imgur.com/IQd3k8i.gif)

### Sample code

```c++
#include <QtGui>
#include <QMapboxGL>

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    QFile data("/usr/share/mbgl/styles/emerald-v7.json");
    data.open(QFile::ReadOnly);

    QTextStream style(&data);

    QMapboxGL map;
    map.setAccessToken(qgetenv("MAPBOX_ACCESS_TOKEN"));
    map.setStyleJSON(style.readAll());

    map.resize(800, 600);
    map.show();

    return app.exec();
}
```

### Build instructions for Ubuntu Vivid or Debian Jessie

##### Install the Mapbox GL development packages:

```
$ sudo apt-add-repository ppa:tmpsantos/mapbox-gl
$ sudo apt-get update
$ sudo apt-install libmbgl-dev
```
##### Clone this repository and build:

```
$ git clone https://github.com/tmpsantos/qmapboxgl.git
$ cd qmapboxgl
$ mkdir build
$ cd build
$ qmake ..
$ make
```

##### Run the example:

You need to export `MAPBOX_ACCESS_TOKEN` if you are using vector tiles hosted at Mapbox servers. Get one access token for free [here](https://www.mapbox.com/).

```
$ MAPBOX_ACCESS_TOKEN=pk.XXXXXXXXXXXX ./qmapboxgl
```
