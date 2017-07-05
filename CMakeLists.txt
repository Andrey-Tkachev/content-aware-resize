project(ContentAwareResize)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

enable_testing()
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

add_subdirectory(config)
add_subdirectory(core)
add_subdirectory(filters)
add_subdirectory(interface)
add_subdirectory(utils)
add_subdirectory(tests)

add_subdirectory(cui)