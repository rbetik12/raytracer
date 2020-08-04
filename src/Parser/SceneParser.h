#pragma once


#include <string>
#include <vector>

namespace RayTracer {
    class SceneParser {
    public:
        static void Parse(const std::string& filename);
    private:
        static std::vector<std::string> Split(const std::string& str);
    };
}

