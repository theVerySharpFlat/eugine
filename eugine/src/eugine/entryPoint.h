//
// Created by aiden on 8/2/21.
//

#ifndef EUGINE_ENTRYPOINT_H
#define EUGINE_ENTRYPOINT_H

#include "log.h"

#ifdef EG_PLATFORM_LINUX

extern eg::Application* eg::createApplication();

int main(int argc, char** argv) {
    eg::Log::init();
    EG_CORE_INFO("welcome to Eugine!");
    auto app = eg::createApplication();
    app -> run();
    delete app;
}

#endif

#endif //EUGINE_ENTRYPOINT_H
