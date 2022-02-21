#pragma once

#include "../components/Camera.hpp"
#include "../components/PlayerInput.hpp"
#include "../components/RigidBody.hpp"
#include "../components/Transform.hpp"
#include <entityx/entityx.h>

using namespace std;
using namespace entityx;

struct CameraSystem : public System<CameraSystem> {
    void update(EntityManager &entityManager, EventManager &eventManager, TimeDelta delta) override {
        entityManager.each<Camera, Transform, PlayerInput, RigidBody>(
            [delta](Entity entity, Camera &camera, Transform &transform, PlayerInput &input, RigidBody &rigidbody) {
                camera.pitch += input.mouse_delta_y * camera.config.sensitivity_pitch;
                camera.pitch = min(+1.5708f, camera.pitch);
                camera.pitch = max(-1.5708f, camera.pitch);
                camera.yaw += input.mouse_delta_x * camera.config.sensitivity_yaw;
                transform.rotation =
                    glm::angleAxis(camera.pitch, glm::vec3(1, 0, 0)) *
                    glm::angleAxis(camera.yaw, glm::vec3(0, 1, 0));
                camera.view =
                    glm::mat4_cast(glm::normalize(transform.rotation)) *
                    glm::translate(glm::mat4(1.0f), -transform.position - glm::vec3(0, camera.config.offset_y, 0));
                auto forward = -glm::vec3(camera.view[0][2], camera.view[1][2], camera.view[2][2]);
                auto right = glm::vec3(camera.view[0][0], camera.view[1][0], camera.view[2][0]);
                auto up = glm::vec3(0, 1, 0);
                auto axis = glm::normalize(glm::vec3(input.axis_forward, input.axis_right, 1));
                auto axis_forward = axis.x;
                auto axis_right = axis.y;
                float speed = camera.config.movement_speed;
                transform.position +=
                    forward * axis_forward * speed * (float)delta +
                    right * axis_right * speed * (float)delta +
                    up * input.axis_up * speed * (float)delta;
                rigidbody.set_position(transform.position);
            });
    };
};
