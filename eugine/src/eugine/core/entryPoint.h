//
// Created by aiden on 8/2/21.
//

#ifndef EUGINE_ENTRYPOINT_H
#define EUGINE_ENTRYPOINT_H

#include "eugine/core/log.h"

extern eg::Application* eg::createApplication();


int main() {
    auto app = eg::createApplication();
    app -> init();
    app -> run();
    delete app;
}


#endif //EUGINE_ENTRYPOINT_H
