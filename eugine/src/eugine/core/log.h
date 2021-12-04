//
// Created by aiden on 8/2/21.
//

#ifndef EUGINE_LOG_H
#define EUGINE_LOG_H


#include "eugine/core/core.h"
#include <spdlog/spdlog.h>

namespace eg {

    class EG_API Logger {
    public:
        static void init();

        inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger;}
        inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }

        template<typename ... Args>
        void trace(const Args&... args) const{
            ::eg::Logger::getClientLogger()->trace(args...);
        }

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;

    };

    extern const ::eg::Logger* log;

#ifdef EG_BUILDING_LIB
    template<typename ... Args>
    void trace(const Args&... args) {
        ::eg::Logger::getCoreLogger()->trace(args...);
    }

    template<typename ... Args>
    void info(const Args&... args) {
        ::eg::Logger::getCoreLogger()->info(args...);
    }

    template<typename ... Args>
    void warn(const Args&... args) {
        ::eg::Logger::getCoreLogger()->warn(args...);
    }

    template<typename ... Args>
    void error(const Args&... args) {
        ::eg::Logger::getCoreLogger()->error(args...);
    }

    template<typename ... Args>
    void fatal(const Args&... args) {
        ::eg::Logger::getCoreLogger()->critical(args...);
    }
#else
    template<typename ... Args>
    void trace(const Args&... args) {
        ::eg::Logger::getClientLogger()->trace(args...);
    }
    
    template<typename ... Args>
    void info(const Args&... args) {
        ::eg::Logger::getClientLogger()->info(args...);
    }
    
    template<typename ... Args>
    void warn(const Args&... args) {
        ::eg::Logger::getClientLogger()->warn(args...);
    }
    
    template<typename ... Args>
    void error(const Args&... args) {
        ::eg::Logger::getClientLogger()->error(args...);
    }
    
    template<typename ... Args>
    void fatal(const Args&... args) {
        ::eg::Logger::getClientLogger()->critical(args...);
    }
#endif
}

#endif //EUGINE_LOG_H
