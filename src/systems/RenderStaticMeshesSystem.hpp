#pragma once

#include "../components/StaticMesh.hpp"
#include "../components/Transform.hpp"
#include "../core/RenderContext.hpp"

#include <entityx/entityx.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <map>
#include <tuple>

using namespace std;
using namespace entityx;

struct RenderStaticMeshesSystem : public System<RenderStaticMeshesSystem>,
                                  public Receiver<RenderStaticMeshesSystem> {
    typedef GLuint vao_t;
    typedef GLuint vbo_vert_t;
    typedef GLuint vbo_uv_t;
    typedef GLuint texture_t;
    typedef GLuint vbo_trans_t;

    typedef tuple<vao_t, vbo_vert_t, vbo_uv_t, texture_t> mesh_family_t;

    map<mesh_family_t, vbo_trans_t> mesh_family_transform_vbos;
    map<mesh_family_t, set<Entity::Id>> mesh_family_members;
    set<mesh_family_t> updated_mesh_families;

    bool should_update_transform_vbos = false;

    void configure(EventManager &event_mgr) override final {
        event_mgr.subscribe<ComponentAddedEvent<StaticMesh>>(*this);
        event_mgr.subscribe<ComponentRemovedEvent<StaticMesh>>(*this);
    }

    void update(EntityManager &entity_mgr, EventManager &event_mgr, TimeDelta delta) override final {
        if (should_update_transform_vbos) {
            update_transform_vbos(entity_mgr);
            updated_mesh_families.clear();
            should_update_transform_vbos = false;
        }
        auto camera = RenderContext.get_camera();
        glm::mat4 vp = camera->proj * camera->view;
        glUniformMatrix4fv(RenderContext.default_shader_vp, 1, GL_FALSE, &vp[0][0]);
        for (auto &[family, vbo_transforms] : mesh_family_transform_vbos) {
            auto [vao, vbo_vertices, vbo_uvs, texture] = family;
            auto meshes_count = mesh_family_members[family].size();
            RenderContext.bind_vao(vao);
            RenderContext.bind_vertices_vbo(vbo_vertices);
            RenderContext.bind_uvs_vbo(vbo_uvs);
            RenderContext.bind_transforms_vbo(vbo_transforms);
            RenderContext.bind_texture(texture);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 36, meshes_count);
        }
    }

    void receive(const ComponentAddedEvent<StaticMesh> &event) {
        auto &mesh = *event.component;
        auto family = tuple(mesh.vao, mesh.vbo_vertices, mesh.vbo_uvs, mesh.texture);
        mesh_family_members[family].insert(event.entity.id());
        updated_mesh_families.insert(family);
        should_update_transform_vbos = true;
    }

    void receive(const ComponentRemovedEvent<StaticMesh> &event) {
        auto &mesh = *event.component;
        auto family = tuple(mesh.vao, mesh.vbo_vertices, mesh.vbo_uvs, mesh.texture);
        mesh_family_members[family].erase(event.entity.id());
        updated_mesh_families.insert(family);
        should_update_transform_vbos = true;
    }

    void update_transform_vbos(EntityManager &entity_mgr) {
        for (auto family : updated_mesh_families) {
            auto members = mesh_family_members[family];
            auto new_trans_buffer_size = members.size() * 3;
            auto new_trans_buffer = new float[new_trans_buffer_size];
            GLuint transforms_vbo;
            if (mesh_family_transform_vbos.contains(family)) {
                transforms_vbo = mesh_family_transform_vbos[family];
            } else {
                glGenBuffers(1, &transforms_vbo);
                mesh_family_transform_vbos[family] = transforms_vbo;
            }
            int i = 0;
            for (auto id : members) {
                auto entity = entity_mgr.get(id);
                auto transform = *(entity.component<Transform>());
                auto position = transform.position;
                new_trans_buffer[i++] = position.x;
                new_trans_buffer[i++] = position.y;
                new_trans_buffer[i++] = position.z;
            }
            glBindBuffer(GL_ARRAY_BUFFER, transforms_vbo);
            glBufferData(GL_ARRAY_BUFFER, new_trans_buffer_size * sizeof(float), new_trans_buffer, GL_STATIC_DRAW);
            delete new_trans_buffer;
        }
    }
};
