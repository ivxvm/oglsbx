#pragma once

#include "../components/StaticMesh.hpp"
#include "../components/Transform.hpp"
#include "../core/EntityDef.hpp"
#include "../utils/load_texture.hpp"
#include <OpenGL/gl3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>

struct GrassTag {};

struct GrassBlock : public EntityDef {
    GLuint texture;
    GrassBlock() {
        texture = load_texture("grass.png");
    }
    Entity spawn_entity(EntityX &ex, Transform transform) const final {
        auto entity = ex.entities.create();
        entity.assign<GrassTag>();
        entity.assign_from_copy<Transform>(transform);
        entity.assign<StaticMesh>(RenderContext.default_vao,
                                  RenderContext.vbo_cube_vertices,
                                  RenderContext.vbo_cube_uvs_grasslike,
                                  texture);
        return entity;
    }
};
