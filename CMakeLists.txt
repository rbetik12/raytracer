cmake_minimum_required(VERSION 3.16)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        vendor/stb
        include
)

set(STB vendor/stb/stb_image.h
        vendor/stb/stb_image_resize.h
        vendor/stb/stb_image_write.h
        )

add_executable(RayTracer ${STB}
        include/geometry.h
        src/Renderer.cpp
        src/Renderer.h
        src/Light.cpp
        src/Light.h
        src/Image.cpp
        src/Image.h
        src/Shapes/Shape.cpp
        src/Shapes/Shape.h
        src/Material.cpp src/Material.h src/Shapes/Sphere.cpp src/Shapes/Sphere.h Application.cpp src/RenderAction.h src/Parser/ArgParser.cpp src/Parser/ArgParser.h src/Parser/Args.h src/Parser/SceneParser.cpp src/Parser/SceneParser.h)

target_link_libraries(RayTracer pthread)