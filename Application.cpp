#include "src/Renderer.h"
#include "src/Parser/ArgParser.h"

int main(int argc, char* argv[]) {
    Args args = RayTracer::ArgParser::Parse(argc, argv);

    RayTracer::Image bg = RayTracer::Image(args.backgroundImage);
    RayTracer::Renderer renderer(args.width, args.height, args.outputFile, bg, M_PI / 2, args.nthreads);

    renderer.AddLight(RayTracer::Light(Vec3f(-20, 20, -20), 2.5));
    renderer.AddLight(RayTracer::Light(Vec3f(-20, 20, 20), 0.5));

    renderer.Render();
    return 0;
}
