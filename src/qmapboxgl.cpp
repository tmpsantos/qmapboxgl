#include "qmapboxgl.h"
#include "qmapboxgl_p.h"

#include <QGLContext>
#include <QMouseEvent>
#include <QWheelEvent>

#include <mbgl/map/map.hpp>
#include <mbgl/platform/gl.hpp>
#include <mbgl/storage/default_file_source.hpp>

QMapboxGL::QMapboxGL(QGLContext *context, QWidget *parent)
    : QGLWidget(context, parent)
    , d_ptr(new QMapboxGLPrivate(context, this))
{
    context->create();
}

QMapboxGL::~QMapboxGL()
{
    delete d_ptr;
}

void QMapboxGL::setAccessToken(const QString &token)
{
    d_ptr->fileSourceObj.setAccessToken(token.toUtf8().constData());
}

void QMapboxGL::setStyleJSON(const QString &style)
{
    d_ptr->mapObj.setStyleJSON(style.toUtf8().constData());
}

double QMapboxGL::latitude() const
{
    return d_ptr->mapObj.getLatLng().latitude;
}

void QMapboxGL::setLatitude(double latitude)
{
    d_ptr->mapObj.setLatLng({latitude, longitude()});
}

double QMapboxGL::longitude() const
{
    return d_ptr->mapObj.getLatLng().longitude;
}

void QMapboxGL::setLongitude(double longitude)
{
    d_ptr->mapObj.setLatLng({latitude(), longitude});
}

double QMapboxGL::zoom() const
{
    return d_ptr->mapObj.getZoom();
}

void QMapboxGL::setZoom(double zoom)
{
    d_ptr->mapObj.setZoom(zoom);
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
        d_ptr->mapObj.moveBy(dx, dy);
    }

    ev->accept();
}

void QMapboxGL::wheelEvent(QWheelEvent *ev)
{
    int numDegrees = ev->delta() / 8;

    if (numDegrees > 0) {
        d_ptr->mapObj.scaleBy(1.10, ev->x(), ev->y());
    } else {
        d_ptr->mapObj.scaleBy(0.90, ev->x(), ev->y());
    }

    ev->accept();
}

void QMapboxGL::resizeGL(int w, int h)
{
    d_ptr->size = { static_cast<uint16_t>(w), static_cast<uint16_t>(h) };
    d_ptr->mapObj.update(mbgl::Update::Dimensions);
}

QMapboxGLPrivate::QMapboxGLPrivate(QGLContext *context_, QMapboxGL *q)
    : contextIsCurrent(false)
    , context(context_)
    , fileSourceObj(nullptr)
    , mapObj(*this, fileSourceObj)
    , q_ptr(q)
{
    connect(this, SIGNAL(viewInvalidated(void)), this, SLOT(triggerRender()), Qt::QueuedConnection);
}

QMapboxGLPrivate::~QMapboxGLPrivate()
{
}

float QMapboxGLPrivate::getPixelRatio() const
{
    return 1.0;
}

std::array<uint16_t, 2> QMapboxGLPrivate::getSize() const
{
    return size;
}

std::array<uint16_t, 2> QMapboxGLPrivate::getFramebufferSize() const
{
    return size;
}

void QMapboxGLPrivate::activate()
{
    // Map thread
    //mbgl::gl::InitializeExtensions([](const char *name) {
    //    const QGLContext *thisContext = QGLContext::currentContext();
    //    return thisContext->getProcAddress(name);
    //});
}

void QMapboxGLPrivate::deactivate()
{
    // Map thread
    context->doneCurrent();
}

void QMapboxGLPrivate::notify()
{
    // Map thread
}

void QMapboxGLPrivate::invalidate()
{
    // Map thread
    if (!contextIsCurrent) {
        context->makeCurrent();
        contextIsCurrent = true;
    }

    emit viewInvalidated();
}

void QMapboxGLPrivate::swap()
{
    // Map thread
    context->swapBuffers();
}

void QMapboxGLPrivate::triggerRender() {
    mapObj.renderSync();
}

#include "moc_qmapboxgl_p.cpp"
