set(UTILS_SOURCE
                 ${CMAKE_CURRENT_SOURCE_DIR}/io.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/singleton.cpp
    PARENT_SCOPE)


set(UI_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../../include/resizable_label.h
              ${CMAKE_CURRENT_SOURCE_DIR}/../../include/imageviewer.h
              ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ui_components.cpp
    PARENT_SCOPE)