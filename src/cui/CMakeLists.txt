set(CONSOLE_SOURCES
        cui.cpp
        #console_handler.cpp
        ${CORE_SOURCE}
        ${FILTERS_SOURCE}
        ${INTERFACE_SOURCE}
        ${UTILS_SOURCE}
        ${CONFIG_SOURCE}
        )

add_executable(xinar ${CONSOLE_SOURCES})

target_link_libraries(xinar boost_program_options
                            boost_system
                            boost_filesystem)
target_link_libraries(xinar ${OpenCV_LIBS})
