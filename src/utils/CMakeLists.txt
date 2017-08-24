set(UTILS_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/io.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
        ../include/utils.h
        ../include/singleton.h

        PARENT_SCOPE)


set(UI_SOURCE ../include/resizable_label.h
        ../include/imageviewer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ui_components.cpp
        PARENT_SCOPE)

set(CONFIG_SOURCE ../include/config.h ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp PARENT_SCOPE)