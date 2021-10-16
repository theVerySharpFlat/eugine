project(imgui)

add_library(imgui SHARED)
target_sources(imgui PRIVATE imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/imgui.cpp)
target_include_directories(imgui PUBLIC imgui)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})