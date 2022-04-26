set(SPDLOG_BUILD_SHARED ON)
add_subdirectory(spdlog)
target_compile_definitions(eugine PUBLIC "GLFW_INCLUDE_NONE")
add_subdirectory(glad)
add_subdirectory(glfw)
target_include_directories(eugine PRIVATE glfw/include)

##imgui doesn't contain a CMakeLists.txt so we have to configure it here
#add_library(imgui STATIC imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp)
#target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
#target_include_directories(eugine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(imgui)
add_subdirectory(glm)
add_subdirectory(filesystem)
add_subdirectory(whereami)
add_subdirectory(stbi)

#target_include_directories(eugine PUBLIC glm)