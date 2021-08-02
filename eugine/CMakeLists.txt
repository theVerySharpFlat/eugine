project(eugine-engine)

set(SHARED_OR_STATIC "STATIC")
if(${EG_BUILD_SHARED})
    message("building shared libraries")
    set(SHARED_OR_STATIC "SHARED")
endif()

add_library(eugine ${SHARED_OR_STATIC} src/eugine.h src/eugine.cpp src/eugine/application.cpp src/eugine/application.h src/eugine/core.h)

if(${EG_BUILD_SHARED})
    target_compile_definitions(eugine PRIVATE EG_BUILD_SHARED_LIB)
endif()