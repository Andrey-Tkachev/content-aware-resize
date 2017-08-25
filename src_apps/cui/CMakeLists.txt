set(CONSOLE_SOURCES
        cui.cpp
        ${UTILS_SOURCE}
        ${CONFIG_SOURCE}
        )

add_executable(cxinar ${CONSOLE_SOURCES})

target_link_libraries(cxinar boost_program_options
                             boost_system
                             boost_filesystem)
target_link_libraries(cxinar ${OpenCV_LIBS} xinar)
