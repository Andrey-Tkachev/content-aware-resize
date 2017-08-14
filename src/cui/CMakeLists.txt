set(CONSOLE_SOURCES
        console.cpp
        ${CORE_SOURCE}
        ${FILTERS_SOURCE}
        ${INTERFACE_SOURCE}
        ${UTILS_SOURCE})

add_executable(xinar ${CONSOLE_SOURCES})

target_link_libraries(xinar boost_program_options)
target_link_libraries(xinar ${OpenCV_LIBS})
