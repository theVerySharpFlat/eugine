//
// Created by aiden on 4/29/22.
//

#ifndef EUGINE_CAMERA2D_H
#define EUGINE_CAMERA2D_H

#include "Camera.h"

namespace eg::rendering {
    class Camera2D : public Camera {
    public:
        /**
         * Create a Camera2D
         * @param dimensions the dimensions(width and height) of the projection
         * @param position  the location of the camera
         */
        Camera2D(glm::vec2 dimensions, glm::vec2 position);

        glm::mat4 getProjection() const override;
        glm::mat4 getView() const override;
        glm::mat4 getProjectionTimesView() const override;

        void setPosition(glm::vec2 position);
        void moveCamera(glm::vec2 offset) { setPosition(m_position + offset); }
        glm::vec2 getPosition() const { return m_position; }

    private:
        glm::mat4 m_projection;
        glm::mat4 m_view;
        glm::mat4 m_projectionTimesView;
        glm::vec2 m_position;

        void recalculateView();

    };
}

#endif //EUGINE_CAMERA2D_H
