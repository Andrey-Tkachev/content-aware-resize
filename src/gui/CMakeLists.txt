# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

set(CMAKE_AUTOMOC ON)


# Use the Widgets module from Qt 5.
add_executable(uxinar uxinar.cpp)
target_link_libraries(uxinar ${OpenCV_LIBS}
                             IMVIEWER
                             INTERFACE_LIB
                             IO_LIB
                             CONFIG_LIB
                             CORE_LIB
                             FILTERS_LIB
                             Qt5::Widgets
                             Qt5::Core)
