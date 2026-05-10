#include "MemoryManager.h"
#include <iostream>
#include <algorithm>

MemoryManager::MemoryManager(int memorySize, const std::vector<Hole> &initialHoles)
    : memorySize(memorySize), holes(initialHoles)
{
    std::sort(holes.begin(), holes.end(), [](const Hole &a, const Hole &b)
              { return a.start < b.start; });
}

std::vector<Hole> MemoryManager::getHoles() const
{
    return holes;
}

std::vector<Process> MemoryManager::getAllocatedProcesses() const
{
    return allocatedProcesses;
}

bool MemoryManager::allocateProcess(Process process, AllocationMethod method)
{
    for (auto &segment : process.segments)
    {
        bool allocated = false;
        if (method == AllocationMethod::FirstFit)
        {
            allocated = allocateSegmentFirstFit(segment);
        }
        else if (method == AllocationMethod::BestFit)
        {
            allocated = allocateSegmentBestFit(segment);
        }

        if (!allocated)
        {
            rollbackProcess(process);
            return false;
        }
    }

    allocatedProcesses.push_back(process);
    return true;
}

bool MemoryManager::allocateSegmentFirstFit(Segment &segment)
{
    for (int i = 0; i < holes.size(); i++)
    {
        if (holes[i].size >= segment.size)
        {
            segment.baseAddress = holes[i].start;

            holes[i].start += segment.size;
            holes[i].size -= segment.size;

            if (holes[i].size == 0)
            {
                holes.erase(holes.begin() + i);
            }

            return true;
        }
    }

    return false;
}

bool MemoryManager::allocateSegmentBestFit(Segment &segment)
{
    int bestIndex = -1;
    for (int i = 0; i < holes.size(); i++)
    {
        if (holes[i].size >= segment.size)
        {
            if (bestIndex == -1 || holes[i].size < holes[bestIndex].size)
            {
                bestIndex = i;
            }
        }
    }

    if (bestIndex != -1)
    {
        segment.baseAddress = holes[bestIndex].start;

        holes[bestIndex].start += segment.size;
        holes[bestIndex].size -= segment.size;

        if (holes[bestIndex].size == 0)
        {
            holes.erase(holes.begin() + bestIndex);
        }

        return true;
    }

    return false;
}

void MemoryManager::mergeHoles()
{
    if (holes.size() <= 1)
        return;

    std::sort(holes.begin(), holes.end(), [](const Hole &a, const Hole &b)
              { return a.start < b.start; });

    for (int i = 0; i < static_cast<int>(holes.size()) - 1; i++)
    {
        if (holes[i].start + holes[i].size == holes[i + 1].start)
        {
            holes[i].size += holes[i + 1].size;
            holes.erase(holes.begin() + i + 1);
            i--;
        }
    }
}

bool MemoryManager::deallocateProcess(const std::string &processName)
{
    for (int i = 0; i < allocatedProcesses.size(); i++)
    {

        if (allocatedProcesses[i].name == processName)
        {

            for (const Segment &segment : allocatedProcesses[i].segments)
            {
                holes.push_back({segment.baseAddress, segment.size});
            }

            allocatedProcesses.erase(allocatedProcesses.begin() + i);

            mergeHoles();

            return true;
        }
    }

    return false;
}

void MemoryManager::rollbackProcess(Process& process){
    for (const auto& segment : process.segments) {
        if (segment.baseAddress != -1) {
            holes.push_back({segment.baseAddress, segment.size});
        }
    }
    mergeHoles();

}