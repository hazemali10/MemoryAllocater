# MemoryAllocater

A modern C++ and Qt-based memory management simulator that demonstrates segmented memory allocation using the **First Fit** and **Best Fit** allocation strategies.

---

## Overview

This project simulates dynamic memory allocation in operating systems using segmentation. The application allows users to:

- Initialize memory with custom holes
- Allocate and deallocate processes dynamically
- Simulate segmented memory allocation
- Visualize memory layout changes in real time
- Compare **First Fit** and **Best Fit** allocation techniques
- Automatically merge adjacent holes after deallocation
- Handle allocation rollback when a process cannot be fully allocated

The project was implemented using:
- **C++** for the core memory management logic
- **Qt Widgets** for the graphical user interface

---

# Features

- Segmented memory allocation
- First Fit allocation strategy
- Best Fit allocation strategy
- Dynamic process creation
- Multiple segments per process
- Hole merging after deallocation
- Rollback mechanism for failed allocations
- Interactive multi-page GUI
- Memory layout visualization
- Process segment tables

---

# GUI Preview

## Memory Initialization Page

<img width="272" height="219" alt="image" src="https://github.com/user-attachments/assets/64ba85f8-5e3d-4792-9ec6-42129677c73c" />

---

## Process Management Page

<img width="468" height="376" alt="image" src="https://github.com/user-attachments/assets/88d394d1-719b-4233-b797-7e26ef5b67e9" />

---

## Memory Layout Output

<img width="468" height="293" alt="image" src="https://github.com/user-attachments/assets/92f81985-2df7-424b-bfcb-46b7713c47e3" />

---

# Project Structure

```text
MemoryAllocater/
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── MemoryManager.cpp
├── MemoryManager.h
├── Process.h
├── Segment.h
├── Hole.h
├── CMakeLists.txt
└── README.md
