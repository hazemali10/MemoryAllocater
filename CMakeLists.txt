cmake_minimum_required(VERSION 3.19)
project(MemoryAllocater LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(MemoryAllocater
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Hole.h
    Segment.h
    Process.h
    MemoryManager.h
    MemoryManager.cpp
)

target_link_libraries(MemoryAllocater
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS MemoryAllocater
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MemoryAllocater
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
