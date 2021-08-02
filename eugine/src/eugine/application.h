//
// Created by aiden on 8/2/21.
//

#ifndef EUGINE_APPLICATION_H
#define EUGINE_APPLICATION_H

#include "core.h"


namespace eg {
    class EG_API Application {
    public:
        Application();

        virtual ~Application();

        void run();
    private:
    };

    Application* createApplication();
}

#endif //EUGINE_APPLICATION_H
