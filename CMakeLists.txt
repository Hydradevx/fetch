cmake_minimum_required(VERSION 3.15)

project(HyprFetch
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

find_package(yaml-cpp REQUIRED)

add_executable(hyprfetch ${SRC_FILES})

target_include_directories(hyprfetch PRIVATE include)

target_link_libraries(hyprfetch PRIVATE yaml-cpp::yaml-cpp)