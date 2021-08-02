//
// Created by aiden on 8/2/21.
//

#ifndef EUGINE_CORE_H
#define EUGINE_CORE_H

#if defined(EG_PLATFORM_WINDOWS)

    #ifdef EG_BUILD_SHARED_LIB
        #define EG_API __declspec(dllexport)
    #else
        #define EG_API __declspec(dllimport)
    #endif
#elif defined(EG_PLATFORM_LINUX)
    #ifdef EG_BUILD_SHARED_LIB
        #define EG_API __attribute__((visibility("default")))
    #else
        #define EG_API
    #endif
#elif defined(EG_PLATFORM_DARWIN)
    //pray this works
    #ifdef EG_BUILD_SHARED_LIB
        #define EG_API __attribute__((visibility("default")))
    #endif
#endif

#endif //EUGINE_CORE_H
