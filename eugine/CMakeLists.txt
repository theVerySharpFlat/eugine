project(eugine-engine)

if(NOT EG_BUILD_TYPE)
    set(EG_BUILD_TYPE SHARED)
endif()

add_library(eugine ${EG_BUILD_TYPE})

if(EG_BUILD_TYPE STREQUAL SHARED)
    target_compile_definitions(eugine PRIVATE EG_BUILD_TYPE_SHARED)

else()
    target_compile_definitions(eugine PRIVATE EG_BUILD_TYPE_STATIC)
endif()
target_compile_definitions(eugine PRIVATE EG_BUILDING_LIB)

find_package(OpenGL REQUIRED)

target_link_libraries(eugine PUBLIC spdlog glad glm::glm)

target_include_directories(eugine PRIVATE vendor/glfw/include)

target_link_libraries(eugine PRIVATE glfw OpenGL::GL imgui)

add_subdirectory(src)
add_subdirectory(vendor)