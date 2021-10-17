//
// Created by aiden on 8/2/21.
//

#ifndef EUGINE_LOG_H
#define EUGINE_LOG_H


#include "eugine/core/core.h"
#include <spdlog/spdlog.h>
#include "glm/gtc/matrix_transform.hpp"

namespace eg {

    class EG_API Log {
    public:
        static void init();

        inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger;}
        inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;

    };

#ifdef EG_BUILDING_LIB
    template<typename ... Args>
    void trace(const Args&... args) {
        ::eg::Log::getCoreLogger()->trace(args...);
    }

    template<typename ... Args>
    void info(const Args&... args) {
        ::eg::Log::getCoreLogger()->info(args...);
    }

    template<typename ... Args>
    void warn(const Args&... args) {
        ::eg::Log::getCoreLogger()->warn(args...);
    }

    template<typename ... Args>
    void error(const Args&... args) {
        ::eg::Log::getCoreLogger()->error(args...);
    }

    template<typename ... Args>
    void fatal(const Args&... args) {
        ::eg::Log::getCoreLogger()->critical(args...);
    }
#else
    template<typename ... Args>
    void trace(const Args&... args) {
        ::eg::Log::getClientLogger()->trace(args...);
    }
    
    template<typename ... Args>
    void info(const Args&... args) {
        ::eg::Log::getClientLogger()->info(args...);
    }
    
    template<typename ... Args>
    void warn(const Args&... args) {
        ::eg::Log::getClientLogger()->warn(args...);
    }
    
    template<typename ... Args>
    void error(const Args&... args) {
        ::eg::Log::getClientLogger()->error(args...);
    }
    
    template<typename ... Args>
    void fatal(const Args&... args) {
        ::eg::Log::getClientLogger()->critical(args...);
    }
#endif
}

#endif //EUGINE_LOG_H
