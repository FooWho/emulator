cmake_minimum_required(VERSION 3.10.0)

project(spaceInvaders VERSION 0.1.0 LANGUAGES C CXX)

add_executable(spaceInvaders)

target_include_directories(spaceInvaders PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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
    src/spaceInvaders/spaceInvaders.cpp
    src/spaceInvaders/spaceInvadersBus.cpp
    src/spaceInvaders/invadersShiftRegister.cpp
    src/spaceInvaders/spaceInvadersButtonDeck.cpp
    src/spaceInvaders/invadersAudio.cpp
)   

target_link_libraries(spaceInvaders PRIVATE
    sfml-audio
    sfml-graphics
    sfml-window
    sfml-system
)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/roms/spaceInvaders/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/roms/spaceInvaders" FILES_MATCHING PATTERN "*.bin")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/roms/spaceInvaders/audio/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/roms/spaceInvaders/audio" FILES_MATCHING PATTERN "*.wav")

add_executable(lunarRescue)

target_include_directories(lunarRescue PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lunarRescue
    ${CMAKE_CURRENT_SOURCE_DIR}/include/spaceInvaders
)

target_sources(lunarRescue PRIVATE
    src/bus.cpp
    src/intel8080.cpp
    src/intel8080Opcodes.cpp
    src/rom.cpp
    src/ram.cpp
    src/dummyPeripheral.cpp
    src/lunarRescue/mainLunarRescue.cpp
    src/lunarRescue/lunarRescue.cpp
    src/spaceInvaders/invadersShiftRegister.cpp
    src/spaceInvaders/spaceInvadersButtonDeck.cpp
)

target_link_libraries(lunarRescue PRIVATE sfml-graphics)
target_link_libraries(lunarRescue PRIVATE sfml-window)
target_link_libraries(lunarRescue PRIVATE sfml-system)

File(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/roms/lunarRescue/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/roms/lunarRescue" FILES_MATCHING PATTERN "*.bin")

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

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/roms/TST8080.BIN" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/roms")
