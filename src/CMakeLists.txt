set( PREPROCESS_SOURCE img_preprocess.cpp )
set( RESIZE_SOURCE img_resize.cpp )

add_library( PREPROCESS_LIB SHARED ${PREPROCESS_SOURCE} )
add_library( RESIZE_LIB SHARED ${RESIZE_SOURCE} )

add_subdirectory(GUI)