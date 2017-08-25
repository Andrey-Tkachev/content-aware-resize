find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)

set(GUI_SOURCES
        uxinar.cpp
        ${UTILS_SOURCE}
        ${UI_SOURCE}
        )

add_executable(uxinar ${GUI_SOURCES})
target_link_libraries(uxinar ${OpenCV_LIBS}
                             Qt5::Widgets
                             Qt5::Core
                             xinar)
