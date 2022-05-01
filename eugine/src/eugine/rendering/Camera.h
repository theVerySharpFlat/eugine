//
// Created by aiden on 4/29/22.
//

#ifndef EUGINE_CAMERA_H
#define EUGINE_CAMERA_H

#include <glm/glm.hpp>

namespace eg::rendering {
    class Camera {
    public:
        virtual glm::mat4 getProjection() const = 0;
        virtual glm::mat4 getView() const = 0;
        virtual glm::mat4 getProjectionTimesView() const = 0;
    };
}

#endif //EUGINE_CAMERA_H
