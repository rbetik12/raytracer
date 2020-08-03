#pragma once

#include <string>
#include <iostream>

struct Args {
    int height;
    int width;
    int nthreads;
    std::string outputFile;
    std::string backgroundImage;

    explicit Args(const int height = 768, const int width = 768, const int nthreads = 8, const std::string& output = "output.png") :
    height(height),
    width(width),
    nthreads(nthreads),
    outputFile(output) {}

    friend std::ostream& operator<< (std::ostream& out, const Args& args) {
        out << args.width << " " << args.height << " " << args.nthreads << " " << args.outputFile;
        return out;
    }
};
