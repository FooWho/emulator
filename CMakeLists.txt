cmake_minimum_required(VERSION 3.10.0)
project(Intel8080 VERSION 0.1.0 LANGUAGES C CXX)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(Intel8080 
    main.cpp
    src/intel8080.cpp
    src/virtualMemory.cpp
    src/rom.cpp
    src/ram.cpp
    src/bus.cpp)

include(CTest)
enable_testing()

