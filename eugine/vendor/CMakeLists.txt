set(SPDLOG_BUILD_SHARED ON)
add_subdirectory(spdlog)
target_compile_definitions(eugine PUBLIC "GLFW_INCLUDE_NONE")
add_subdirectory(glad)
add_subdirectory(glfw)

#imgui doesn't contain a CMakeLists.txt so we have to configure it here
add_library(imgui SHARED imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

add_subdirectory(glm)

#target_include_directories(eugine PUBLIC glm)