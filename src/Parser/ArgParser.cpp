#include <stdexcept>
#include "ArgParser.h"
#include <thread>

Args RayTracer::ArgParser::Parse(int argc, char** argv) {
    Args args = Args();

    args.width = 1920;
    args.height = 1080;
    args.outputFile = "output.png";
    args.nthreads = 8;
    args.backgroundImage = "";

    for (size_t i = 1; i < argc; i++) {
        const std::string& argToken = argv[i];
        if (argToken.size() <= 2) throw std::invalid_argument("Cannot parse argument: " + argToken);
        if (argToken.substr(2) == std::string("width")) {
            args.width = std::stoi(argv[++i]);;
        } else if (argToken.substr(2) == std::string("height")) {
            args.height = std::stoi(argv[++i]);
        } else if (argToken.substr(2) == std::string("nthreads")) {
            args.nthreads = std::stoi(argv[++i]);
        } else if (argToken.substr(2) == std::string("output")) {
            args.outputFile = argv[++i];
        } else if (argToken.substr(2) == std::string("bg")) {
            args.backgroundImage = argv[++i];
        } else {
            throw std::invalid_argument("Unknown command line argument: " + argToken);
        }
    }

    ValidateConsoleArguments(args);
    return args;
}

void RayTracer::ArgParser::ValidateConsoleArguments(Args args) {
    if (args.width % 2 != 0 || args.height % 2 != 0) {
        throw std::invalid_argument("Output image dimensions must be even");
    }
    if (args.nthreads <= 0 || args.nthreads > std::thread::hardware_concurrency()) {
        throw std::invalid_argument("Incorrect threads amount. Must be between 1 and " + std::to_string(std::thread::hardware_concurrency()));
    }
}
