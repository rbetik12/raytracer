#pragma once

#include "Args.h"

namespace RayTracer {
    class ArgParser {
    public:
        static Args Parse(int argc, char* argv[]);
    private:
        static void ValidateConsoleArguments(Args args);
    };
}