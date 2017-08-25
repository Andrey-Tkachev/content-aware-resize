project(ContentAwareResize)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Release)
include_directories(include_xinar include_apps)

add_subdirectory(src_xinar)
add_subdirectory(src_apps)
