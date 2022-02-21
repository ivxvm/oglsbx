#pragma once

#include "../components/Camera.hpp"
#include "../components/PlayerInput.hpp"
#include "../components/RigidBody.hpp"
#include "../components/Transform.hpp"
#include "../core/PhysicsContext.hpp"

#include <bullet/btBulletDynamicsCommon.h>
#include <entityx/entityx.h>

using namespace std;
using namespace entityx;

struct PhysicsSystem : public System<PhysicsSystem> {
    void update(EntityManager &entity_mgr, EventManager &event_mgr, TimeDelta delta) override {
        PhysicContext.dynamics_world->stepSimulation(delta);
        entity_mgr.each<Camera, Transform, RigidBody>(
            [delta](Entity entity, Camera &camera, Transform &transform, RigidBody &rigidbody) {
                btTransform bt_transform;
                rigidbody.ptr->getMotionState()->getWorldTransform(bt_transform);
                const auto &origin = bt_transform.getOrigin();
                transform.position.x = origin.getX();
                transform.position.y = origin.getY();
                transform.position.z = origin.getZ();
            });
    };
};
