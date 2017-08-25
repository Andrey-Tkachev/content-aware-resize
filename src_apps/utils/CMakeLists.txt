set(UTILS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/io.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
        PARENT_SCOPE)

set(CONFIG_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp
        PARENT_SCOPE)
        
set(UI_SOURCE ${CMAKE_SOURCE_DIR}/include_apps/resizable_label.h
              ${CMAKE_SOURCE_DIR}/include_apps/imageviewer.h
              ${CMAKE_CURRENT_SOURCE_DIR}/ui_components.cpp
        PARENT_SCOPE)
