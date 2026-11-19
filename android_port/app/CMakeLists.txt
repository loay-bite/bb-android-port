cmake_minimum_required(VERSION 3.22.1)
project(battlebrothers_native CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2 path configuration for Android
set(SDL2_DIR ${CMAKE_SOURCE_DIR}/../../../../sdl2)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(battlebrothers SHARED
    native-lib.cpp
    squirrel_bridge.cpp
)

find_library(log-lib log)
find_library(android-lib android)
find_library(glesv3-lib GLESv3)
find_library(egl-lib EGL)

target_link_libraries(battlebrothers
    PRIVATE
    ${log-lib}
    ${android-lib}
    ${glesv3-lib}
    ${egl-lib}
)
