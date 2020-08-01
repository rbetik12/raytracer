#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Image.h"

void RayTracer::Image::LoadImage() {
    unsigned char* backgroundImageRaw = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (backgroundImageRaw == nullptr) {
        throw std::invalid_argument("Cannot load file: " + filename);
    }

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            image.push_back(Vec3f(backgroundImageRaw[(i + j * width) * 3], backgroundImageRaw[(i + j * width) * 3 + 1],
                                  backgroundImageRaw[(i + j * width) * 3 + 2]) * (1.f / 255));
        }
    }
    stbi_image_free(backgroundImageRaw);
}
