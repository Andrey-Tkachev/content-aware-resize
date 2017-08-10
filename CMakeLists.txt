project(ContentAwareResize)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

enable_testing()
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
include_directories(include)

add_subdirectory(src/filters)
add_subdirectory(src/utils)
add_subdirectory(src/core)
add_subdirectory(src/interface)
add_subdirectory(src/cui)
add_subdirectory(src/gui)

