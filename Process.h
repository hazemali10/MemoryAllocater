#pragma once
#include <string>
#include <vector>
#include "Segment.h"

struct Process {
    std::string name;
    std::vector<Segment> segments;

    Process(std::string name)
        : name(name) {}
};