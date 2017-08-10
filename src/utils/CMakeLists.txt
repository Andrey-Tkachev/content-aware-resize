find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

set(CMAKE_AUTOMOC ON)
set(UTILS_SOURCE radix.cpp ../../include/io.h io.cpp singleton.cpp ui_components.cpp)

#add_library(SINGLETON_TEMPLATE SHARED radix.cpp)
#add_library(IO_LIB SHARED io.cpp ../../include/io.h)
#add_library(UTILS_LIB SHARED singleton.cpp)
#add_library(IMVIEWER SHARED ui_components.cpp)

#target_link_libraries(IMVIEWER Qt5::Widgets
#                               Qt5::Core)