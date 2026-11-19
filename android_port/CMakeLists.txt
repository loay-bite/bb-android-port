cmake_minimum_required(VERSION 3.22.1)
project(battlebrothers_android CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 (expects SDL2 to be available via FetchContent or Android package)
find_package(SDL2 REQUIRED)

# Squirrel 3 Subdirectory or External
# We include squirrel3 headers and source files if bundled, or link against built libs
add_library(battlebrothers SHARED
    app/src/main/cpp/native-lib.cpp
    app/src/main/cpp/squirrel_bridge.cpp
)

target_include_directories(battlebrothers PRIVATE
    app/src/main/cpp
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(battlebrothers
    PRIVATE
    ${SDL2_LIBRARIES}
    GLESv2
    GLESv3
    EGL
    log
    android
)
