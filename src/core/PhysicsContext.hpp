#pragma once

#include "../components/Transform.hpp"
#include <bullet/btBulletDynamicsCommon.h>
#include <iostream>

struct PhysicsContextSingleton {
    btDiscreteDynamicsWorld *dynamics_world;
    btCollisionShape *block_collision_shape;
    btCollisionShape *creature_collision_shape;

    void initialize() {
        auto collision_cfg = new btDefaultCollisionConfiguration();
        auto collision_dispatcher = new btCollisionDispatcher(collision_cfg);
        auto broadphase = new btDbvtBroadphase();
        auto constraint_solver = new btSequentialImpulseConstraintSolver();
        dynamics_world = new btDiscreteDynamicsWorld(
            collision_dispatcher, broadphase, constraint_solver, collision_cfg);
        dynamics_world->setGravity(btVector3(0, -10, 0));
        block_collision_shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
        creature_collision_shape = new btCylinderShape(btVector3(0.5f, 1.0f, 0.5f));
    }

    btRigidBody *spawn_block_rigidbody(Transform transform) {
        auto rigidbody = new btRigidBody(0.f, NULL, block_collision_shape);
        rigidbody->translate(
            btVector3(transform.position.x,
                      transform.position.y,
                      transform.position.z));
        dynamics_world->addRigidBody(rigidbody);
        return rigidbody;
    }

    btRigidBody *spawn_creature_rigidbody(Transform transform) {
        btScalar mass(1.0f);
        btVector3 local_inertia(0, 0, 0);
        creature_collision_shape->calculateLocalInertia(mass, local_inertia);
        btTransform start_transform;
        start_transform.setIdentity();
        start_transform.setOrigin(btVector3(transform.position.x,
                                            transform.position.y,
                                            transform.position.z));
        auto motion_state = new btDefaultMotionState(start_transform);
        auto rigidbody = new btRigidBody(mass, motion_state, creature_collision_shape, local_inertia);
        rigidbody->setActivationState(DISABLE_DEACTIVATION);
        rigidbody->setAngularFactor(0);
        dynamics_world->addRigidBody(rigidbody);
        return rigidbody;
    }
} PhysicContext;
