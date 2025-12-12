cmake_minimum_required(VERSION 3.15)

project(Fetch VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

add_executable(fetch
    ${SRC_FILES}
)