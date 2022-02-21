#pragma once

#include "../components/PlayerInput.hpp"
#include "../components/RigidBody.hpp"
#include "../components/Transform.hpp"
#include "../core/EntityDef.hpp"
#include "../core/PhysicsContext.hpp"

struct PlayerTag {};

struct Player : public EntityDef {
    Entity spawn_entity(EntityX &ex, Transform transform) const final {
        auto entity = ex.entities.create();
        entity.assign<PlayerTag>();
        entity.assign_from_copy<Transform>(transform);
        entity.assign<Camera>();
        entity.assign<PlayerInput>();
        entity.assign<RigidBody>(PhysicContext.spawn_creature_rigidbody(transform));
        return entity;
    }
};
