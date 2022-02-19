#pragma once

#include "../components/Transform.hpp"
#include "../core/RenderContext.hpp"
#include <entityx/entityx.h>

using namespace entityx;

struct EntityDef {
    virtual Entity spawn_entity(EntityX &, Transform) const = 0;
};
