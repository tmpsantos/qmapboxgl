#ifndef QMAPBOXGL_P_H
#define QMAPBOXGL_P_H

#include <mbgl/map/map.hpp>
#include <mbgl/map/view.hpp>
#include <mbgl/storage/default_file_source.hpp>

namespace mbgl {

class Map;
class FileSource;

}  // namespace mbgl

class QOpenGLContext;

class QMapboxGLPrivate : public mbgl::View
{
public:
    explicit QMapboxGLPrivate(QMapboxGL *q);
    virtual ~QMapboxGLPrivate();

    // mbgl::View implementation.
    void activate() override {}
    void deactivate() override;
    void notify() override {}
    void invalidate(std::function<void()> renderCallback) override;

    mbgl::DefaultFileSource fileSource;
    mbgl::Map map;

    int lastX = 0;
    int lastY = 0;

    QOpenGLContext *context = nullptr;

    QMapboxGL *q_ptr;
};

#endif // QMAPBOXGL_P_H
