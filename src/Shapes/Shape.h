#pragma once


#include <geometry.h>
#include "../Material.h"

namespace RayTracer {
    enum ShapeType {
        ESphere,
        EPlane
    };

    class Shape {
    public:
        const Vec3f position;
        const Material material;
        const ShapeType type;

        Shape(const Vec3f& position, const Material& material, const ShapeType type) : position(position), material(material), type(type) {}

        virtual bool RayIntersect(const Vec3f& orig, const Vec3f& dir, float& t0) = 0;
    };
}