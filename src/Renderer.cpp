#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>
#include "Renderer.h"
#include "Shapes/Sphere.h"
#include <thread>
#include <chrono>

void RayTracer::Renderer::AddLight(const Light& light) {
    lights.push_back(light);
}

void RayTracer::Renderer::AddBackground(const Image& image) {
    background = image;
}

void RayTracer::Renderer::AddShape(const Shape& shape) {
//    std::unique_ptr<Shape> shape_ptr;
//    if (shape.type == ShapeType::ESphere) {
//        shape_ptr = std::make_unique<Sphere>(shape);
//    }
//
//    shapes.push_back(shape_ptr);
}

void RayTracer::Renderer::RenderOnThread() {
    while (true) {
        RenderAction action;

        if (renderQueue.empty()) {
            std::cout << "Queue is empty. Finishing thread ID: " << std::this_thread::get_id() << " execution" << std::endl;
            return;
        } else {
            const std::lock_guard<std::mutex> lock(renderQueue_mutex);
            std::cout << "Locked queue" << std::endl;
            action = renderQueue.front();
            renderQueue.pop();
            std::cout << "Thread with ID: " << std::this_thread::get_id() << " started working on image part." << '\n';
            std::cout << "Queue size: " << renderQueue.size() << std::endl;
        }
        std::cout << "Unlocked queue" << std::endl;


        for (size_t j = action.height1; j > action.height0; j--) {
            for (size_t i = action.width0; i < action.width1; i++) {
                if (i == width || j == height) continue;
                float x = (2 * (i + 0.5) / (float) width - 1) * tan(fov / 2.) * width / (float) height;
                float y = -(2 * (j + 0.5) / (float) height - 1) * tan(fov / 2.);
                Vec3f dir = Vec3f(x, y, -1).normalize();
                Vec3f res = CastRay(Vec3f(0, 0, 0), dir, 0, Vec2f(i, j));
                frameBuffer[i + j * width] = res;
                if (i + j * width >= frameBuffer.size()) {
                    int size = frameBuffer.size();
                    throw std::overflow_error("Framebuffer overflow occurred. Framebuffer size is "
                    + std::to_string(size)
                    + " and trying to get " + std::to_string(i + j * width) + " element.\n"
                    + "X: " + std::to_string(i) + "Y: " + std::to_string(j));
                }
            }
        }

        std::cout << "Thread with ID: " << std::this_thread::get_id() << " finished working on image part." << std::endl;
    }
}

void RayTracer::Renderer::RenderFirstRowOnThread() {
    int j = 0;
    for (size_t i = 0; i < width; i++) {
        float x = (2 * (i + 0.5) / (float) width - 1) * tan(fov / 2.) * width / (float) height;
        float y = -(2 * (j + 0.5) / (float) height - 1) * tan(fov / 2.);
        Vec3f dir = Vec3f(x, y, -1).normalize();
        Vec3f res = CastRay(Vec3f(0, 0, 0), dir, 0, Vec2f(i, j));
        frameBuffer[i + j * width] = res;
    }
}

void RayTracer::Renderer::Render() {
    auto startTime = std::chrono::high_resolution_clock::now();
    const int threadMultiplier = 2;

    int widthStep = width / (threadsNumber * threadMultiplier);
    int heightStep = height / (threadsNumber * threadMultiplier);

    for (size_t i = 1; i <= threadsNumber * threadMultiplier; i++) {
        for (size_t j = 1; j <= threadsNumber * threadMultiplier; j++) {
            renderQueue.push(RenderAction(widthStep * (i - 1), widthStep * i, heightStep * (j - 1), heightStep * j));
        }
    }

    std::vector<std::thread> execThreads;
    for (size_t i = 0; i < threadsNumber; i++) {
        std::thread thread(&RayTracer::Renderer::RenderOnThread, this);

        execThreads.push_back(std::move(thread));
    }

    std::thread thread(&RayTracer::Renderer::RenderFirstRowOnThread, this);
    execThreads.push_back(std::move(thread));

    for (auto& execThread : execThreads) {
        execThread.join();
    }

    std::cout << "Saving image..." << std::endl;
    SaveToPng(3);
    std::cout << "Saved image" << std::endl;
    auto finishTime = std::chrono::high_resolution_clock::now();
    std::cout << "Execution time is: " << duration_cast<std::chrono::milliseconds>(finishTime - startTime).count() << " ms" << std::endl;
}

void RayTracer::Renderer::SaveToPng(const int channels) const {
    auto* framebufferRaw = new unsigned char[width * height * channels];
    size_t index = 0;

    for (size_t i = 0; i < height * width; i++) {
        for (size_t j = 0; j < channels; j++) {
            Vec3f c = frameBuffer[i];
            float max = std::max(c[0], std::max(c[1], c[2]));
            if (max > 1) {
                c = c * (1.0f / max);
            }
            framebufferRaw[index] = (unsigned char) (255 * std::max(0.f, std::min(1.f, c[j])));
            index += 1;
        }
    }


    stbi_write_png(outputFile.c_str(), width, height, channels, framebufferRaw, width * channels);
}

Vec3f RayTracer::Renderer::CastRay(const Vec3f& orig, const Vec3f& dir, size_t depth, const Vec2f& currentPoint) {
    Vec3f hit, normal;
    Material pixelMaterial;

    if (depth > 4 || !SceneIntersect(orig, dir, pixelMaterial, hit, normal)) {
        Sphere env(Vec3f(0, 0, 0), 100, Material());
        float dist = 0;
        env.RayIntersect(orig, dir, dist);
        Vec3f p = orig + dir * dist;
        int a = (atan2(p.z, p.x) / (2 * M_PI) + .5) * background.GetWidth();
        int b = acos(p.y / 100) / M_PI * background.GetHeight();
        return background.GetImage()[a + b * background.GetWidth()];
    }

    Vec3f reflect_dir = Reflect(dir, normal).normalize();
    Vec3f refract_dir = Refract(dir, normal, pixelMaterial.refractiveIndex).normalize();
    Vec3f reflect_orig =
            reflect_dir * normal < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3; // offset the original point to avoid occlusion by the object itself
    Vec3f refract_orig = refract_dir * normal < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3;
    Vec3f reflect_color = CastRay(reflect_orig, reflect_dir, depth + 1);
    Vec3f refract_color = CastRay(refract_orig, refract_dir, depth + 1);

    float diffuseLightingIntensity = 0, specularLightIntensity = 0;

    for (auto light : lights) {
        Vec3f lightDir = (light.position - hit).normalize();
        float light_distance = (light.position - hit).norm();

        Vec3f shadowOrig =
                lightDir * normal < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3; // checking if the point lies in the shadow of the lights[i]
        Vec3f shadowPoint, shadowNormal;
        Material tmpmaterial;
        if (SceneIntersect(shadowOrig, lightDir, tmpmaterial, shadowPoint, shadowNormal) &&
            (shadowPoint - shadowOrig).norm() < light_distance)
            continue;

        diffuseLightingIntensity += light.intensity * std::max(0.f, lightDir * normal);
        specularLightIntensity += powf(std::max(0.0f, Reflect(lightDir, normal) * dir), pixelMaterial.specularExponent) * light.intensity;
    }

    return pixelMaterial.diffuseColor * diffuseLightingIntensity * pixelMaterial.albedo[0] +
           Vec3f(1, 1, 1) * specularLightIntensity * pixelMaterial.albedo[1] +
           reflect_color * pixelMaterial.albedo[2] +
           refract_color * pixelMaterial.albedo[3];
}

bool RayTracer::Renderer::SceneIntersect(const Vec3f& orig, const Vec3f& dir, Material& pixelMaterial, Vec3f& hit,
                                         Vec3f& normal) const {

    Material ivory(1.0, Vec4f(0.6, 0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3), 50.);
    Material glass(1.5, Vec4f(0.0, 0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8), 125.);
    Material red_rubber(1.0, Vec4f(0.9, 0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1), 10.);
    Material mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3f(-5, 0, -16), 2, ivory));
    spheres.push_back(Sphere(Vec3f(-3, -1.5, -12), 2, glass));
    spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, red_rubber));
    spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, mirror));

    float closestSphereDist = std::numeric_limits<float>::max();
    for (auto sphere: spheres) {
        float sphereDist;
        if (sphere.RayIntersect(orig, dir, sphereDist) && sphereDist < closestSphereDist) {
            closestSphereDist = sphereDist;
            hit = orig + dir * closestSphereDist;
            normal = (hit - sphere.position).normalize();
            pixelMaterial = sphere.material;
        }
    }

    float checkerboard_dist = std::numeric_limits<float>::max();
    if (fabs(dir.y) > 1e-3) {
        float d = -(orig.y + 4) / dir.y; // the checkerboard plane has equation y = -4
        Vec3f pt = orig + dir * d;
        if (d > 0 && fabs(pt.x) < 10 && pt.z < -10 && pt.z > -30 && d < closestSphereDist) {
            checkerboard_dist = d;
            hit = pt;
            normal = Vec3f(0, 1, 0);
            pixelMaterial.diffuseColor = (int(.5 * hit.x + 1000) + int(.5 * hit.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(1, .7, .3);
            pixelMaterial.diffuseColor = pixelMaterial.diffuseColor * .3;
        }
    }

    return std::min(closestSphereDist, checkerboard_dist) < 1000;
}

Vec3f RayTracer::Renderer::Refract(const Vec3f& I, const Vec3f& N, float refractiveIndex) const {
    float cosi = -std::max(-1.f, std::min(1.f, I * N));
    float etai = 1, etat = refractiveIndex;
    Vec3f n = N;
    if (cosi < 0) { // if the ray is inside the object, swap the indices and invert the normal to get the correct result
        cosi = -cosi;
        std::swap(etai, etat);
        n = -N;
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vec3f(0, 0, 0) : I * eta + n * (eta * cosi - sqrtf(k));
}

Vec3f RayTracer::Renderer::Reflect(const Vec3f& I, const Vec3f& N) const {
    return I - N * 2.f * (I * N);
}
