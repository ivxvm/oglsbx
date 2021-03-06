#pragma once

#include <OpenGL/gl3.h>

#include "../components/Camera.hpp"
#include "../utils/load_shaders.hpp"

class RenderContextSingleton {
private:
    const Camera *camera;
    GLuint active_vao = 0;
    GLuint active_vertices_vbo = 0;
    GLuint active_uvs_vbo = 0;
    GLuint active_transforms_vbo = 0;
    GLuint active_texture = 0;

public:
    bool is_initialized = false;
    GLuint default_vao;
    GLuint vbo_cube_vertices;
    GLuint vbo_cube_uvs_alldiff;
    GLuint vbo_cube_uvs_grasslike;
    GLuint default_shader;
    GLint default_shader_vp;

    void set_camera(const Camera *camera) {
        this->camera = camera;
    }

    const Camera *get_camera() {
        assert(camera != NULL);
        return camera;
    }

    void bind_vao(GLuint vao) {
        if (active_vao != vao) {
            glBindVertexArray(vao);
            active_vao = vao;
        }
    }

    void bind_vertices_vbo(GLuint vertices_vbo, GLuint attrib_index = 0) {
        if (active_vertices_vbo != vertices_vbo) {
            glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
            glVertexAttribPointer(attrib_index, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
            glEnableVertexAttribArray(attrib_index);
            active_vertices_vbo = vertices_vbo;
        }
    }

    void bind_uvs_vbo(GLuint uvs_vbo, GLuint attrib_index = 1) {
        if (active_uvs_vbo != uvs_vbo) {
            glBindBuffer(GL_ARRAY_BUFFER, uvs_vbo);
            glVertexAttribPointer(attrib_index, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
            glEnableVertexAttribArray(attrib_index);
            active_uvs_vbo = uvs_vbo;
        }
    }

    void bind_transforms_vbo(GLuint transforms_vbo, GLuint attrib_index = 2) {
        if (active_transforms_vbo != transforms_vbo) {
            glBindBuffer(GL_ARRAY_BUFFER, transforms_vbo);
            glVertexAttribPointer(attrib_index, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
            glVertexAttribDivisor(attrib_index, 1);
            glEnableVertexAttribArray(attrib_index);
            active_transforms_vbo = transforms_vbo;
        }
    }

    void bind_texture(GLuint texture) {
        if (active_texture != texture) {
            glBindTexture(GL_TEXTURE_2D, texture);
            active_texture = texture;
        }
    }

    void initialize() {
        glGenVertexArrays(1, &default_vao);
        glBindVertexArray(default_vao);
        // --------------------------------------------------------------------
        glGenBuffers(1, &vbo_cube_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
        const GLfloat cube_vertices[] =
            {// front
             0.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f,
             1.0f, 0.0f, 0.0f,
             1.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f,
             1.0f, 1.0f, 0.0f,
             // left
             1.f, 0.f, 0.f,
             1.f, 1.f, 0.f,
             1.f, 0.f, -1.f,
             1.f, 0.f, -1.f,
             1.f, 1.f, 0.f,
             1.f, 1.f, -1.f,
             // back
             1.0f, 0.0f, -1.0f,
             1.0f, 1.0f, -1.0f,
             0.0f, 0.0f, -1.0f,
             0.0f, 0.0f, -1.0f,
             1.0f, 1.0f, -1.0f,
             0.0f, 1.0f, -1.0f,
             // right
             0.0f, 0.0f, -1.0f,
             0.0f, 1.0f, -1.0f,
             0.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 0.0f,
             0.0f, 1.0f, -1.0f,
             0.0f, 1.0f, 0.0f,
             // up
             0.0f, 1.0f, 0.0f,
             0.0f, 1.0f, -1.0f,
             1.0f, 1.0f, 0.0f,
             1.0f, 1.0f, 0.0f,
             0.0f, 1.0f, -1.0f,
             1.0f, 1.0f, -1.0f,
             // down
             1.0f, 0.0f, 0.0f,
             1.0f, 0.0f, -1.0f,
             0.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 0.0f,
             1.0f, 0.0f, -1.0f,
             0.0f, 0.0f, -1.0f};
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
        // --------------------------------------------------------------------
        glGenBuffers(1, &vbo_cube_uvs_alldiff);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_uvs_alldiff);
        const GLfloat cube_uvs_alldiff[] =
            {// front
             0. / 6., 1.,
             0. / 6., 0.,
             1. / 6., 1.,
             1. / 6., 1.,
             0. / 6., 0.,
             1. / 6., 0.,
             // left
             1. / 6., 1.,
             1. / 6., 0.,
             2. / 6., 1.,
             2. / 6., 1.,
             1. / 6., 0.,
             2. / 6., 0.,
             // back
             2. / 6., 1.,
             2. / 6., 0.,
             3. / 6., 1.,
             3. / 6., 1.,
             2. / 6., 0.,
             3. / 6., 0.,
             // right
             3. / 6., 1.,
             3. / 6., 0.,
             4. / 6., 1.,
             4. / 6., 1.,
             3. / 6., 0.,
             4. / 6., 0.,
             // up
             4. / 6., 1.,
             4. / 6., 0.,
             5. / 6., 1.,
             5. / 6., 1.,
             4. / 6., 0.,
             5. / 6., 0.,
             // down
             5. / 6., 1.,
             5. / 6., 0.,
             6. / 6., 1.,
             6. / 6., 1.,
             5. / 6., 0.,
             6. / 6., 0.};
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_uvs_alldiff), cube_uvs_alldiff, GL_STATIC_DRAW);
        // --------------------------------------------------------------------
        glGenBuffers(1, &vbo_cube_uvs_grasslike);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_uvs_grasslike);
        const GLfloat cube_uvs_grasslike[] =
            {// front
             2. / 3., 1.,
             2. / 3., 0.,
             3. / 3., 1.,
             3. / 3., 1.,
             2. / 3., 0.,
             3. / 3., 0.,
             // left
             2. / 3., 1.,
             2. / 3., 0.,
             3. / 3., 1.,
             3. / 3., 1.,
             2. / 3., 0.,
             3. / 3., 0.,
             // back
             2. / 3., 1.,
             2. / 3., 0.,
             3. / 3., 1.,
             3. / 3., 1.,
             2. / 3., 0.,
             3. / 3., 0.,
             // right
             2. / 3., 1.,
             2. / 3., 0.,
             3. / 3., 1.,
             3. / 3., 1.,
             2. / 3., 0.,
             3. / 3., 0.,
             // up
             0. / 3., 1.,
             0. / 3., 0.,
             1. / 3., 1.,
             1. / 3., 1.,
             0. / 3., 0.,
             1. / 3., 0.,
             // down
             1. / 3., 1.,
             1. / 3., 0.,
             2. / 3., 1.,
             2. / 3., 1.,
             1. / 3., 0.,
             2. / 3., 0.};
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_uvs_grasslike), cube_uvs_grasslike, GL_STATIC_DRAW);
        // --------------------------------------------------------------------
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        // --------------------------------------------------------------------
        default_shader = load_shaders("src/shaders/block.vert", "src/shaders/block.frag");
        default_shader_vp = glGetUniformLocation(default_shader, "vp");
        is_initialized = true;
    }
} RenderContext;
