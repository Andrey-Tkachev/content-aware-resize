# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

set(CMAKE_AUTOMOC ON)

set(GUI_SOURCES
        uxinar.cpp
        ../../include/core.h
        ../core/core.cpp
        ../../include/filters.h
        ../filters/filters.cpp
        ../../include/interface.h
        ../interface/interface.cpp
        ../../include/io.h
        ../utils/io.cpp
        ../../include/matrixwrapper.h
        ../utils/matrixwrapper.cpp
        #../utils/radix.cpp
        ../utils/singleton.cpp
        ../utils/ui_components.cpp
        )

# Use the Widgets module from Qt 5.
add_executable(uxinar ${GUI_SOURCES})
target_link_libraries(uxinar ${OpenCV_LIBS}
                             #IMVIEWER
                             #INTERFACE_LIB
                             #IO_LIB
                             #CORE_LIB
                             #FILTERS_LIB
                             Qt5::Widgets
                             Qt5::Core)
