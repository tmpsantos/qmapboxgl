#include "example.h"

#include <QApplication>
#include <QFile>
#include <QPushButton>
#include <QTextStream>
#include <QVBoxLayout>

#include <QMapboxGL>

MapController::MapController(QMapboxGL *map)
    : m_map(map)
    , m_latitudeAnimation(map, "latitude")
    , m_longitudeAnimation(map, "longitude")
    , m_zoomAnimation(map, "zoom")
{
    m_map->setAccessToken(qgetenv("MAPBOX_ACCESS_TOKEN"));
    changeStyle();

    // Show the Nordic countries.
    m_map->setLatitude(63.761821);
    m_map->setLongitude(17.744822);
    m_map->setZoom(3.5);

    // Define the annimation parameters
    m_latitudeAnimation.setEasingCurve(QEasingCurve::OutQuint);
    m_latitudeAnimation.setDuration(5000);

    m_longitudeAnimation.setEasingCurve(QEasingCurve::OutQuint);
    m_longitudeAnimation.setDuration(5000);

    m_zoomAnimation.setDuration(5000);
}

MapController::~MapController()
{
}

QString MapController::getStyle()
{
    static unsigned currentStyle;
    const char *styles[] {
        ":/emerald-v7.json",
        ":/mapbox-streets-v7.json",
        ":/light-v7.json",
        ":/dark-v7.json",
    };

    if (currentStyle >= sizeof(styles) / sizeof(char *)) {
        currentStyle = 0;
    }

    QFile data(styles[++currentStyle - 1]);
    data.open(QFile::ReadOnly);

    QTextStream style(&data);
    return style.readAll();
}

void MapController::changeStyle()
{
    m_map->setStyleJSON(getStyle());
}

void MapController::setPosition()
{
    if (m_zoomAnimation.state() == QAbstractAnimation::Running) {
        return;
    }

    // Helsinki city center.
    m_latitudeAnimation.setEndValue(60.170448);
    m_longitudeAnimation.setEndValue(24.942046);
    m_zoomAnimation.setEndValue(14.0);

    m_latitudeAnimation.start();
    m_longitudeAnimation.start();
    m_zoomAnimation.start();
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QVBoxLayout *layout = new QVBoxLayout;

    QMapboxGL *map = new QMapboxGL;
    layout->addWidget(map);

    MapController mapController(map);

    QPushButton *styleButton = new QPushButton("&Change map style");
    layout->addWidget(styleButton);
    QObject::connect(styleButton, SIGNAL(clicked(void)), &mapController, SLOT(changeStyle(void)));

    QPushButton *jumpButton = new QPushButton("&Take me to Helsinki");
    layout->addWidget(jumpButton);
    QObject::connect(jumpButton, SIGNAL(clicked(void)), &mapController, SLOT(setPosition(void)));

    QPushButton *quitButton = new QPushButton("&Quit");
    layout->addWidget(quitButton);
    QObject::connect(quitButton, SIGNAL(clicked()), &app, SLOT(quit()));

    QWidget window;
    window.setLayout(layout);
    window.resize(800, 600);
    window.show();
    window.setWindowTitle("Mapbox GL for Qt");

    return app.exec();
}

#include "moc_example.cpp"
