#pragma once

#include <OpenGL/gl3.h>

#include "load_shaders.hpp"

struct RenderContext {
    GLuint vao;
    GLuint vbo_cube_vertices;
    GLuint vbo_cube_uvs_alldiff;
    GLuint default_shader;
    GLint default_shader_mvp;

    RenderContext();
};

RenderContext::RenderContext() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // --------------------------------------------------------------------
    default_shader = load_shaders("src/shaders/block.vert", "src/shaders/block.frag");
    default_shader_mvp = glGetUniformLocation(default_shader, "mvp");
}
