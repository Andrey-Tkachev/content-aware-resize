# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

set(CMAKE_AUTOMOC ON)

# Tell CMake to create the helloworld executable
add_library(IMVIEWER SHARED imageviewer.cpp)
target_link_libraries(IMVIEWER Qt5::Widgets Qt5::Core FILTERS_LIB CORE )

add_executable(uxinar graphics.cpp)
# Use the Widgets module from Qt 5.
target_link_libraries(uxinar IMVIEWER Qt5::Widgets ${OpenCV_LIBS})