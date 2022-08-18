//
// Created by aiden on 8/18/22.
//

#ifndef EUGINE_SCENE_H
#define EUGINE_SCENE_H

#include "entt/entt.hpp"

namespace eg {
    class Scene {
    public:
        Scene(){};
        ~Scene(){};

    private:
        entt::registry m_registry;
    };
}


#endif //EUGINE_SCENE_H
