#pragma once

#include <cstddef>
#include <iostream>

struct RenderAction {
    size_t width0;
    size_t width1;

    size_t height0;
    size_t height1;

    RenderAction(size_t width0, size_t width1, size_t height0, size_t height1) : width0(width0), width1(width1), height0(height0), height1(height1) {}

    RenderAction(): width0(0), width1(0), height0(0), height1(0){}

    friend std::ostream& operator<<(std::ostream& out, const RenderAction& action) {
        out << "Width0: " << action.width0 << " Width1: " << action.width1 << " Height0: " << action.height0 << " Height1: "
        << action.height1;
        return out;
    }
};