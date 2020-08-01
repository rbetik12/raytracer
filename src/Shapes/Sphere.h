#pragma once


#include <geometry.h>
#include "../Material.h"
#include "Shape.h"

namespace RayTracer {
    class Sphere : public Shape {
    public:
        float radius;

        Sphere(const Vec3f& center, const float r, const Material& material) : Shape(center, material, ShapeType::ESphere), radius(r) {
        }

        bool RayIntersect(const Vec3f& orig, const Vec3f& dir, float& t0) override {
            Vec3f L = position - orig;
            float tca = L * dir;
            float d2 = L * L - tca * tca;
            if (d2 > radius * radius) return false;
            float thc = sqrtf(radius * radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) t0 = t1;
            return t0 >= 0;
        }
    };
}

