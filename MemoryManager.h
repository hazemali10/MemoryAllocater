#pragma once

#include <vector>
#include <string>
#include "Hole.h"
#include "Process.h"

enum class AllocationMethod {
    FirstFit,
    BestFit
};

class MemoryManager {
private:
    int memorySize;
    std::vector<Hole> holes;
    std::vector<Process> allocatedProcesses;

    bool allocateSegmentFirstFit(Segment& segment);
    bool allocateSegmentBestFit(Segment& segment);
    void rollbackProcess(Process& process);

public:
    MemoryManager(int memorySize, const std::vector<Hole>& initialHoles);

    bool allocateProcess(Process process, AllocationMethod method);
    bool deallocateProcess(const std::string& processName);

    void mergeHoles();

    std::vector<Hole> getHoles() const;
    std::vector<Process> getAllocatedProcesses() const;


};