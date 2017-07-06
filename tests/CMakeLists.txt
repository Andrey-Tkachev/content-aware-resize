find_package (Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


# Test for Core
set(TCore test_core)
set(CORE_SOURCES core_test.cpp)
add_executable(${TCore} ${CORE_SOURCES})
target_link_libraries(${TCore} CORE ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_test(${TCore} ${TCore})
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

add_subdirectory(test_build)