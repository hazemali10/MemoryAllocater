#pragma once
#include <string>

struct Segment {
    std::string name;
    int size;
    int baseAddress;

    Segment(std::string name, int size)
        : name(name), size(size), baseAddress(-1) {}
};