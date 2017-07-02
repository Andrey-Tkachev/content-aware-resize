project(ContentAwareResize)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )
find_package(Boost REQUIRED)

enable_testing()
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
add_subdirectory(src)
#add_subdirectory(tests)


#add_executable(resize ${SRC_LIST})
#target_link_libraries( resize ${OpenCV_LIBS})