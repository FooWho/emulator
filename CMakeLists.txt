cmake_minimum_required(VERSION 3.10.0)

project(Intel8080 VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.15.0
)
FetchContent_MakeAvailable(spdlog)

add_executable(Intel8080 
    main.cpp
    src/intel8080.cpp
    src/intel8080_opcodes.cpp
    src/virtualMemory.cpp
    src/rom.cpp
    src/ram.cpp
    src/bus.cpp
)
target_include_directories(Intel8080 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    spdlog
)
target_link_libraries(Intel8080 
    spdlog::spdlog
)

enable_testing()

add_executable(
    emuTest
    emuTest.cpp
    src/intel8080.cpp
    src/intel8080_opcodes.cpp
    src/virtualMemory.cpp
    src/rom.cpp
    src/ram.cpp
    src/bus.cpp
)
target_include_directories(emuTest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    spdlog
)
target_link_libraries(
  emuTest
  spdlog::spdlog
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(emuTest)

