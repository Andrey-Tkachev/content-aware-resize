set(PREPROCESS_SOURCE img_preprocess.cpp CUI/main.cpp)
set( RESIZE_SOURCE img_resize.cpp )

add_library( PREPROCESS_LIB SHARED ${PREPROCESS_SOURCE} )
add_library( RESIZE_LIB SHARED ${RESIZE_SOURCE} )
target_link_libraries(RESIZE_LIB PREPROCESS_LIB)
add_subdirectory(GUI)
add_subdirectory(CUI)

