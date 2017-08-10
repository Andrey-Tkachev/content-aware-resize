#add_executable(xinar console.cpp ${INTERFACE_SOURCE} ${CORE_SOURCES} ${FILTERS_SOURCE} ${UTILS_SORUCE})
set(CONSOLE_SOURCES
        console.cpp
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
        )
add_executable(xinar ${CONSOLE_SOURCES})


target_link_libraries(xinar boost_program_options)
target_link_libraries(xinar ${OpenCV_LIBS})
#target_link_libraries(xinar INTERFACE_LIB)
#target_link_libraries(xinar SINGLETON_TEMPLATE)
#target_link_libraries(xinar IO_LIB)
#target_link_libraries(xinar FILTERS_LIB)
#target_link_libraries(xinar CORE_LIB)
#target_link_libraries(xinar UTILS_LIB)


