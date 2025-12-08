cmake_minimum_required(VERSION 3.10.0)

project(Intel8080 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Intel8080 
    src/main.cpp
    src/intel8080.cpp
    src/intel8080Opcodes.cpp
    src/cpu.cpp
    src/virtualMemory.cpp
    src/rom.cpp
    src/ram.cpp
    src/bus.cpp
    src/spaceInvaders.cpp
)
target_include_directories(Intel8080 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


