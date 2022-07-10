//
// Created by aiden on 4/29/22.
//

#include "Camera2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace eg::rendering {
    Camera2D::Camera2D(glm::vec2 dimensions, glm::vec2 position) {
        m_projection = glm::ortho(-dimensions.x / 2, dimensions.x / 2,
                                  -dimensions.y / 2, dimensions.y / 2,
                                  0.0f, 1.0f);
        m_position = position;
        recalculateView();
        m_projectionTimesView = m_projection * m_view;
    }

    void Camera2D::setPosition(glm::vec2 pos) {
        m_position = pos;
        recalculateView();
        m_projectionTimesView = m_projection * m_view;
    }

    void Camera2D::recalculateView() {
        m_view = glm::lookAt(glm::vec3(m_position, 1.0f), glm::vec3(m_position, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 Camera2D::getProjection() const {
        return m_projection;
    }

    glm::mat4 Camera2D::getView() const {
        return m_view;
    }

    glm::mat4 Camera2D::getProjectionTimesView() const {
        return m_projectionTimesView;
    }
}