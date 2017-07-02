find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(BOOST_LIBRARYDIR ${BOOST_LIB})
set(SOURCE main.cpp)
set(MAIN main)

add_executable(${MAIN} ${SOURCE})
target_link_libraries(${MAIN} PREPROCESS_LIB ${Boost_LIBRARIES} ${OpenCV_LIBS} RESIZE_LIB)

add_definitions(-DBoost_USE_STATIC_LIBS=ON)