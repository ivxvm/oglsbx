#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

struct RigidBody {
    btRigidBody *ptr;

    RigidBody(btRigidBody *bt_rigidbody_ptr)
        : ptr(bt_rigidbody_ptr) {}

    void set_position(const glm::vec3 &position) {
        btTransform bt_transform;
        bt_transform.setIdentity();
        bt_transform.setOrigin(btVector3(position.x, position.y, position.z));
        ptr->setWorldTransform(bt_transform);
        ptr->getMotionState()->setWorldTransform(bt_transform);
    };
};
