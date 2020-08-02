#pragma once


#include <memory>
#include <queue>
#include <mutex>
#include "Light.h"
#include "Image.h"
#include "Shapes/Shape.h"
#include "RenderAction.h"

namespace RayTracer {
    class Renderer {
    public:
        const int width;
        const int height;

        Renderer(const int width, const int height, const std::string& output, const Image& background = Image(), const float fov = M_PI / 2, const int nthreads = 8) : width(
                width), height(height), outputFile(output), fov(fov), threadsNumber(nthreads) {
            frameBuffer = std::vector<Vec3f>(height * width);
        }

        ~Renderer() {}

        void Render();

        void AddLight(const Light& light);

        void AddBackground(const Image& image);

        void AddShape(const Shape& shape);

    private:
        std::vector<std::unique_ptr<Shape>> shapes;
        std::vector<Light> lights;
        std::vector<Vec3f> frameBuffer;
        float fov;
        Image background;
        const std::string outputFile;
        std::queue<RenderAction> renderQueue;
        int threadsNumber;
        std::mutex renderQueue_mutex;

        void RenderOnThread();

        void SaveToPng(const int channels) const;

        Vec3f CastRay(const Vec3f& orig, const Vec3f& dir, size_t depth = 0, const Vec2f& currentPoint = Vec2f());

        bool SceneIntersect(const Vec3f& orig, const Vec3f& dir, Material& pixelMaterial, Vec3f& hit,
                            Vec3f& normal) const;

        Vec3f Refract(const Vec3f& I, const Vec3f& N, float refractiveIndex) const;

        Vec3f Reflect(const Vec3f& I, const Vec3f& N) const;
    };
}


