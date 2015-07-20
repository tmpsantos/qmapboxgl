#ifndef QMAPBOXGL_P_H
#define QMAPBOXGL_P_H

#include <QObject>
#include <QScopedPointer>

#include <mbgl/map/map.hpp>
#include <mbgl/map/view.hpp>
#include <mbgl/storage/default_file_source.hpp>

namespace mbgl {

class Map;
class FileSource;

}  // namespace mbgl

class QOpenGLContext;

class QMapboxGLPrivate : public QObject, public mbgl::View
{
    Q_OBJECT

public:
    explicit QMapboxGLPrivate(QMapboxGL *q);
    virtual ~QMapboxGLPrivate();

    // mbgl::View implementation.
    float getPixelRatio() const override;
    std::array<uint16_t, 2> getSize() const override;
    std::array<uint16_t, 2> getFramebufferSize() const override;
    void initialize(mbgl::Map *map) override;
    void activate() override;
    void deactivate() override;
    void notify() override;
    void invalidate() override;
    void swap() override;

    int lastX = 0;
    int lastY = 0;

    std::array<uint16_t, 2> size;

    QScopedPointer<QOpenGLContext> context;

    mbgl::DefaultFileSource fileSourceObj;
    mbgl::Map mapObj;

    QMapboxGL *q_ptr;

signals:
    void viewInvalidated();

public slots:
    void triggerRender();
};

#endif // QMAPBOXGL_P_H
