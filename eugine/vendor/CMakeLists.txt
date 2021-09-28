set(SPDLOG_BUILD_SHARED ON)
add_subdirectory(spdlog)
target_compile_definitions(eugine PUBLIC "GLFW_INCLUDE_NONE")
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(glm)

#target_include_directories(eugine PUBLIC glm)