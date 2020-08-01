#pragma once

#include <geometry.h>

namespace RayTracer {
    class Image {
    public:
        Image() {
            empty = true;
        }

        Image(const std::string& filename) : filename(filename) {
            empty = false;
            LoadImage();
        }

        const std::vector<Vec3f>& GetImage() {
            return image;
        }

        inline const int GetWidth() const {
            return width;
        }

        inline const int GetHeight() const {
            return height;
        }

        inline const int GetChannels() const {
            return channels;
        };

        inline const bool IsEmpty() const {
            return empty;
        }

    private:
        int width;
        int height;
        int channels;
        std::string filename;
        bool empty;
        std::vector<Vec3f> image;

        void LoadImage();
    };
}