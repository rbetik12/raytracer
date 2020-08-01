#include "src/Renderer.h"

int main() {
    RayTracer::Renderer renderer(768, 768, "out.png");

    renderer.AddLight(RayTracer::Light(Vec3f(-20, 20, -20), 2.5));
    renderer.AddLight(RayTracer::Light(Vec3f(-20, 20, 20), 0.5));

    RayTracer::Image bg = RayTracer::Image("../res/bg.jpg");

    renderer.AddBackground(bg);

    renderer.Render();
    return 0;
}
