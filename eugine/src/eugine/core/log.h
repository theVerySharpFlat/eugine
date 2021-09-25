//
// Created by aiden on 8/2/21.
//

#ifndef EUGINE_LOG_H
#define EUGINE_LOG_H


#include "eugine/core/core.h"
#include <spdlog/spdlog.h>

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

}



//core log
#define EG_CORE_TRACE(...) ::eg::Log::getCoreLogger()->trace(__VA_ARGS__)
#define EG_CORE_INFO(...)  ::eg::Log::getCoreLogger()->info(__VA_ARGS__)
#define EG_CORE_WARN(...)  ::eg::Log::getCoreLogger()->warn(__VA_ARGS__)
#define EG_CORE_ERROR(...) ::eg::Log::getCoreLogger()->error(__VA_ARGS__)
#define EG_CORE_FATAL(...) ::eg::Log::getCoreLogger()->critical(__VA_ARGS__)


//client logger
#define EG_TRACE(...) ::eg::Log::getClientLogger()->trace(__VA_ARGS__)
#define EG_INFO(...)  ::eg::Log::getClientLogger()->info(__VA_ARGS__)
#define EG_WARN(...)  ::eg::Log::getClientLogger()->warn(__VA_ARGS__)
#define EG_ERROR(...) ::eg::Log::getClientLogger()->error(__VA_ARGS__)
#define EG_FATAL(...) ::eg::Log::getClientLogger()->critical(__VA_ARGS__)



#endif //EUGINE_LOG_H
