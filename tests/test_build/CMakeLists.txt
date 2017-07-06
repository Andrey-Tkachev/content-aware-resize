add_executable(TestBuild main.cpp)

target_link_libraries(TestBuild CORE ${OpenCV_LIBS})
