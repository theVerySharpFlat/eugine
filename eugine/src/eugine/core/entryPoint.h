//
// Created by aiden on 8/2/21.
//

#ifndef EUGINE_ENTRYPOINT_H
#define EUGINE_ENTRYPOINT_H

#include "eugine/core/log.h"

extern eg::Application* eg::createApplication();

#ifndef EG_PLATFORM_WINDOWS


int main(int argc, char** argv) {
    eg::Log::init();
    EG_CORE_INFO("welcome to Eugine!");
    auto app = eg::createApplication();
    app -> init();
    app -> run();
    delete app;
}

#else

int main(int argcm char** argv) {
    eg::Log::init();
    EG_CORE_INFO("welcome to Eugine!");
    auto app = eg::createApplication();
    app -> run();
    delete app;
}

#endif

#endif //EUGINE_ENTRYPOINT_H
