#pragma once


#include <geometry.h>

namespace RayTracer {
    class Material {
    public:
        Vec3f diffuseColor;
        Vec4f albedo;
        float specularExponent;
        float refractiveIndex;

        explicit Material(const float r, const Vec4f& albedo, const Vec3f& color, const float spec) : diffuseColor(color), albedo(albedo),
                                                                                                      specularExponent(spec), refractiveIndex(r) {}

        Material() : diffuseColor(0, 0, 0), albedo(1, 0, 0, 0), specularExponent(), refractiveIndex(1) {}
    };
}