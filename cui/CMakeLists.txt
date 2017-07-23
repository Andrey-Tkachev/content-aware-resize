add_executable(xinar console.cpp)

target_link_libraries(xinar boost_program_options)
target_link_libraries(xinar ${OpenCV_LIBS})
target_link_libraries(xinar INTERFACE_LIB)
target_link_libraries(xinar CONFIG_LIB)
target_link_libraries(xinar SINGLETON_TEMPLATE)
target_link_libraries(xinar IO_LIB)
target_link_libraries(xinar FILTERS_LIB)
target_link_libraries(xinar CORE_LIB)
target_link_libraries(xinar UTILS_LIB)


