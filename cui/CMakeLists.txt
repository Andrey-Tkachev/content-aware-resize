add_executable(resize console.cpp)

target_link_libraries(resize SINGLETON_TEMPLATE)
target_link_libraries(resize CONFIG_LIB)
target_link_libraries(resize IO_LIB)
target_link_libraries(resize INTERFACE_LIB)
target_link_libraries(resize CORE)
target_link_libraries(resize boost_program_options)
target_link_libraries(resize ${OpenCV_LIBS})
