#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QObject>
#include <QPropertyAnimation>

class QMapboxGL;

class MapController : public QObject {
    Q_OBJECT

public:
    MapController(QMapboxGL *map);
    ~MapController();

private:
    QString getStyle();

    QMapboxGL *m_map;

    QPropertyAnimation m_latitudeAnimation;
    QPropertyAnimation m_longitudeAnimation;
    QPropertyAnimation m_zoomAnimation;

public slots:
    void changeStyle();
    void setPosition();
};

#endif // EXAMPLE_H
