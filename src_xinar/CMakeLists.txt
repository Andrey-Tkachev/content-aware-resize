add_subdirectory(core)
add_subdirectory(interface)
add_subdirectory(filters)

add_library(xinar SHARED ${CORE_SOURCE} ${INTERFACE_SOURCE} ${FILTERS_SOURCE})
