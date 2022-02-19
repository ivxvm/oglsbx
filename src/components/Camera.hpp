#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

struct Camera {
    struct {
        float sensitivity_yaw = 0.0075f;
        float sensitivity_pitch = 0.0075f;
        float movement_speed = 20.f;
    } config;

    float pitch = 0;
    float yaw = 0;

    glm::mat4 proj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.f);
    glm::mat4 view;
};
