set(s ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(eugine PRIVATE ${s}/log.cpp ${s}/application.cpp
                              ${s}/application.cpp ${s}/layer.cpp ${s}/layerStack.cpp
                              ${s}/imgui/imguiLayer.cpp)

add_subdirectory(platform/linux)
add_subdirectory(platform/OpenGL)