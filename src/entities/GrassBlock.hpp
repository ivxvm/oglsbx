#pragma once

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
        std::cout << "TEST1 !!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        texture = load_texture("grass.png");
        std::cout << "TEST2 !!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    }
    Entity spawn_entity(EntityX &ex, Transform transform) const final {
        auto entity = ex.entities.create();
        auto entity_transform = entity.assign<Transform>();
        *entity_transform = transform;
        entity.assign<GrassTag>();
        return entity;
    }
    void register_systems(EntityX &ex) const final {
        // ex.systems.add<RenderSystem>();
    }
    void update_systems(EntityX &ex, TimeDelta delta) const final {
        // ex.systems.update<RenderSystem>(delta);
    }
    void render(EntityX &ex, RenderContext &render_ctx, TimeDelta delta) const final {
        render_ctx.bind_default_vao();
        render_ctx.bind_cube_vertices_vbo();
        render_ctx.bind_cube_grasslike_uvs_vbo();
        glBindTexture(GL_TEXTURE_2D, texture);
        glm::mat4 vp = render_ctx.camera.proj * render_ctx.camera.view;
        ex.entities.each<GrassTag, Transform>([&](Entity entity, GrassTag &tag, Transform &transform) {
            // std::cout << "transform: " << transform.position.x << " " << transform.position.z << std::endl;
            glm::mat4 mvp = vp * glm::translate(glm::mat4(1.0), transform.position);
            glUniformMatrix4fv(render_ctx.default_shader_mvp, 1, GL_FALSE, &mvp[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            // transform.position
            // position.x += direction.x * dt
            // position.y += direction.y * dt;
        });
    }

    // private:
    //     struct RenderSystem : public System<RenderSystem> {
    //         GLuint texture = texture;
    //         void update(EntityManager &entity_mgr, EventManager &event_mgr, TimeDelta delta) override {
    //         }
    //     };
};
