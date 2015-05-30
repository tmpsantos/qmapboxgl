#include "qmapboxgl.h"
#include "qmapboxgl_p.h"

#include <QMouseEvent>
#include <QOpenGLContext>
#include <QWheelEvent>

#include <mbgl/map/map.hpp>
#include <mbgl/platform/gl.hpp>
#include <mbgl/storage/default_file_source.hpp>

QMapboxGL::QMapboxGL(QWindow *parent)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
    , d_ptr(new QMapboxGLPrivate(this))
{
    QSurfaceFormat format;
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setStencilBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setDepthBufferSize(16);

    setFormat(format);
}

QMapboxGL::~QMapboxGL()
{
    delete d_ptr;
}

void QMapboxGL::setAccessToken(const QString &token)
{
    d_ptr->map.setAccessToken(token.toUtf8().constData());
}

void QMapboxGL::setStyleJSON(const QString &style)
{
    d_ptr->map.setStyleJSON(style.toUtf8().constData());
}

double QMapboxGL::latitude() const
{
    return d_ptr->map.getLatLng().latitude;
}

void QMapboxGL::setLatitude(double latitude)
{
    d_ptr->map.setLatLng({latitude, longitude()});
}

double QMapboxGL::longitude() const
{
    return d_ptr->map.getLatLng().longitude;
}

void QMapboxGL::setLongitude(double longitude)
{
    d_ptr->map.setLatLng({latitude(), longitude});
}

double QMapboxGL::zoom() const
{
    return d_ptr->map.getZoom();
}

void QMapboxGL::setZoom(double zoom)
{
    d_ptr->map.setZoom(zoom);
}

void QMapboxGL::mousePressEvent(QMouseEvent *ev)
{
    if (!(ev->buttons() & Qt::LeftButton)) {
        return;
    }

    d_ptr->lastX = ev->x();
    d_ptr->lastY = ev->y();

    ev->accept();
}

void QMapboxGL::mouseMoveEvent(QMouseEvent *ev)
{
    if (!(ev->buttons() & Qt::LeftButton)) {
        return;
    }

    int dx = ev->x() - d_ptr->lastX;
    int dy = ev->y() - d_ptr->lastY;

    d_ptr->lastX = ev->x();
    d_ptr->lastY = ev->y();

    if (dx || dy) {
        d_ptr->map.moveBy(dx, dy);
    }

    ev->accept();
}

void QMapboxGL::wheelEvent(QWheelEvent *ev)
{
    QPoint numDegrees = ev->angleDelta();

    if (numDegrees.y() > 0) {
        d_ptr->map.scaleBy(1.10, ev->x(), ev->y());
    } else {
        d_ptr->map.scaleBy(0.90, ev->x(), ev->y());
    }

    ev->accept();
}

void QMapboxGL::resizeGL(int w, int h)
{
    // FIXME: Find out why sprites don't get rendered
    // correctly if I use 1.0 here.
    d_ptr->map.resize(w, h, 1.00001);
}

QMapboxGLPrivate::QMapboxGLPrivate(QMapboxGL *q)
    : fileSource(nullptr)
    , map(*this, fileSource)
    , q_ptr(q)
{
}

QMapboxGLPrivate::~QMapboxGLPrivate()
{
}

void QMapboxGLPrivate::deactivate()
{
    context->doneCurrent();
}

void QMapboxGLPrivate::invalidate(std::function<void()> renderCallback)
{
    if (!q_ptr->isExposed()) {
        return;
    }

    if (!context) {
        context = new QOpenGLContext();
        context->setFormat(q_ptr->requestedFormat());
        context->create();
        context->makeCurrent(q_ptr);

        mbgl::gl::InitializeExtensions([](const char *name) {
            QOpenGLContext *thisContext = QOpenGLContext::currentContext();
            return thisContext->getProcAddress(name);
        });
    }

    renderCallback();

    context->swapBuffers(q_ptr);
}
