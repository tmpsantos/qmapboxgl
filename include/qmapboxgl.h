#ifndef QMAPBOXGL_H
#define QMAPBOXGL_H

#include <QGLWidget>

class QGLContext;
class QMapboxGLPrivate;

class QMapboxGL : public QGLWidget
{
    Q_OBJECT
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude)
    Q_PROPERTY(double zoom READ zoom WRITE setZoom)

public:
    explicit QMapboxGL(QGLContext *context, QWidget *parent = 0);
    virtual ~QMapboxGL();

    void setAccessToken(const QString &token);
    void setStyleJSON(const QString &style);

    double latitude() const;
    void setLatitude(double latitude);

    double longitude() const;
    void setLongitude(double longitude);

    double zoom() const;
    void setZoom(double zoom);

private:
    Q_DISABLE_COPY(QMapboxGL)

    // QOpenGLWindow implementation.
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent * ev);
    void wheelEvent(QWheelEvent * ev);
    void resizeGL(int w, int h);

    QMapboxGLPrivate *d_ptr;
};

#endif // QMAPBOXGL_H
