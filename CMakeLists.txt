cmake_minimum_required(VERSION 3.10.0)

project(spaceInvaders VERSION 0.1.0 LANGUAGES C CXX)

add_executable(spaceInvaders)

target_include_directories(spaceInvaders PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/spaceInvaders
)

target_sources(spaceInvaders PRIVATE
    src/bus.cpp
    src/intel8080.cpp
    src/intel8080Opcodes.cpp
    src/rom.cpp
    src/ram.cpp
    src/dummyPeripheral.cpp
    src/spaceInvaders/mainSpaceInvaders.cpp
    src/spaceInvaders/mainSpaceInvaders.cpp
    src/spaceInvaders/spaceInvaders.cpp
    src/spaceInvaders/spaceInvadersBus.cpp
    src/spaceInvaders/invadersShiftRegister.cpp
    src/spaceInvaders/spaceInvadersButtonDeck.cpp
)   

target_link_libraries(spaceInvaders PRIVATE sfml-graphics)
target_link_libraries(spaceInvaders PRIVATE sfml-window)
target_link_libraries(spaceInvaders PRIVATE sfml-system)

add_executable(cpuTest
    src/mainCPUTest.cpp
    src/intel8080.cpp
    src/intel8080Opcodes.cpp
    src/rom.cpp
    src/ram.cpp
    src/bus.cpp
)

target_include_directories(cpuTest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
