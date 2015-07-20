#include <QtWidgets>
#include <QMapboxGL>

QString getStyle() {
    static unsigned currentStyle;
    const char *styles[] {
        ":/emerald-v7.json",
        ":/mapbox-streets-v7.json",
        ":/light-v7.json",
        ":/dark-v7.json",
        ":/satellite-v7.json",
    };

    if (currentStyle >= sizeof(styles) / sizeof(char *)) {
        currentStyle = 0;
    }

    QFile data(styles[++currentStyle - 1]);
    data.open(QFile::ReadOnly);

    QTextStream style(&data);
    return style.readAll();
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QVBoxLayout *layout = new QVBoxLayout;

    QMapboxGL *map = new QMapboxGL;
    map->setAccessToken(qgetenv("MAPBOX_ACCESS_TOKEN"));
    map->setStyleJSON(getStyle());

    // Show the Nordic countries.
    map->setLatitude(63.761821);
    map->setLongitude(17.744822);
    map->setZoom(3.5);

    QWidget *container = QWidget::createWindowContainer(map);
    layout->addWidget(container);

    QPushButton *styleButton = new QPushButton("&Change map style");
    layout->addWidget(styleButton);
    QObject::connect(styleButton, &QPushButton::clicked, [map]() {
        map->setStyleJSON(getStyle());
    });

    QPropertyAnimation latitudeAnimation(map, "latitude");
    latitudeAnimation.setEasingCurve(QEasingCurve::OutQuint);
    latitudeAnimation.setDuration(5000);

    QPropertyAnimation longitudeAnimation(map, "longitude");
    longitudeAnimation.setEasingCurve(QEasingCurve::OutQuint);
    longitudeAnimation.setDuration(5000);

    QPropertyAnimation zoomAnimation(map, "zoom");
    zoomAnimation.setDuration(5000);

    QPushButton *jumpButton = new QPushButton("&Take me to Helsinki");
    layout->addWidget(jumpButton);
    QObject::connect(jumpButton, &QPushButton::clicked, [&]() {
        if (zoomAnimation.state() == QAbstractAnimation::Running) {
            return;
        }

        // Helsinki city center.
        latitudeAnimation.setEndValue(60.170448);
        longitudeAnimation.setEndValue(24.942046);
        zoomAnimation.setEndValue(14.0);

        latitudeAnimation.start();
        longitudeAnimation.start();
        zoomAnimation.start();
    });

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
