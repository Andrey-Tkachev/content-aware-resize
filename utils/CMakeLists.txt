set(SINGLETON_TEMPLATE_SOURCE singleton.cpp ../include/singleton.h)
set(IO_SOURCE io.cpp ../include/io.h)

add_library(SINGLETON_TEMPLATE SHARED ${SINGLETON_TEMPLATE_SOURCE})
add_library(IO_LIB SHARED ${IO_SOURCE})