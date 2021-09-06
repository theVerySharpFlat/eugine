project(eugine-engine)

set(SHARED_OR_STATIC "SHARED")


add_library(eugine ${SHARED_OR_STATIC})



target_compile_definitions(eugine PRIVATE EG_BUILD_SHARED_LIB)

add_subdirectory(vendor)

find_package(OpenGL REQUIRED)

target_link_libraries(eugine PUBLIC spdlog glad)


target_link_libraries(eugine PRIVATE OpenGL::GL glfw imgui)

add_subdirectory(src)