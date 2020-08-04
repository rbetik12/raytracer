#include "SceneParser.h"
#include "../Shapes/Sphere.h"
#include <fstream>
#include <iostream>
#include <geometry.h>
#include <map>

void RayTracer::SceneParser::Parse(const std::string& filename) {
    std::ifstream sceneDescription(filename);
    std::map<std::string, Material> materials;

    if (!sceneDescription.is_open()) {
        throw std::invalid_argument("Cannot open scene description: " + filename);
    }

    std::string line;
    while (std::getline(sceneDescription, line)) {
        const std::vector<std::string> tokens = Split(line);
        if (tokens.empty()) continue;

        if (tokens[0] == "ViewAt") {
            Vec3f viewDirection(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
            std::cout << "View at" << " " << viewDirection << std::endl;
        } else if (tokens[0] == "Sphere") {
            Sphere sphere(Vec3f(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])), std::stof(tokens[4]), Material());
            std::cout << "Sphere at " << sphere.position << " R: " << sphere.radius << " with material: " << std::endl;
        }
        else {
            std::string matName = tokens[0];
            materials.insert(std::pair<std::string, Material>(matName, Material(std::stof(tokens[1]),
                                                                                Vec4f(std::stof(tokens[2]), std::stof(tokens[3]),
                                                                                      std::stof(tokens[4]), std::stof(tokens[5])),
                                                                                Vec3f(std::stof(tokens[6]), std::stof(tokens[7]),
                                                                                      std::stof(tokens[8])), std::stof(tokens[9]))));

            std::cout << matName << ": "<<materials[matName].diffuseColor << " " << materials[matName].refractiveIndex << " " << materials[matName].albedo << " "
                      << materials[matName].specularExponent << std::endl;
        }
    }
    sceneDescription.close();
}

std::vector<std::string> RayTracer::SceneParser::Split(const std::string& s) {
    std::vector<std::string> tokens;
    std::string delim = " ";

    auto start = 0u;
    auto end = s.find(delim);
    while (end != std::string::npos) {
        auto token = s.substr(start, end - start);
        tokens.push_back(token);
        start = end + delim.length();
        end = s.find(delim, start);
    }

    if (start < s.size()) {
        auto token = s.substr(start, s.size());
        tokens.push_back(token);
    }

    return tokens;
}
