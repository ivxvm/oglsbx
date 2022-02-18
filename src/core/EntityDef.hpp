#pragma once

#include "../components/Transform.hpp"
#include "../core/RenderContext.hpp"
#include <entityx/entityx.h>

using namespace entityx;

struct EntityDef {
    virtual Entity spawn_entity(EntityX &, Transform) const = 0;
    virtual void register_systems(EntityX &) const = 0;
    virtual void update_systems(EntityX &, TimeDelta) const = 0;
    virtual void render(EntityX &, RenderContext &, TimeDelta) const = 0;
};
