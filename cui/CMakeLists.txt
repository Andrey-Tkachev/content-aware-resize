add_executable(CUI console.cpp)

target_link_libraries(CUI SINGLETON_TEMPLATE)
target_link_libraries(CUI CONFIG_LIB)
