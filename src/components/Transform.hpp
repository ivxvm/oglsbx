#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

struct Transform {
    glm::vec3 position;
    glm::quat rotation;

    Transform(float x = 0.f,
              float y = 0.f,
              float z = 0.f,
              float pitch = 0.f,
              float yaw = 0.f,
              float roll = 0.f)
        : position(x, y, z),
          rotation(glm::vec3(pitch, yaw, roll)) {}
};
