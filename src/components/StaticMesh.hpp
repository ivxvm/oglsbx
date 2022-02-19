#pragma once

#include <OpenGL/gl3.h>

struct StaticMesh {
    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_uvs;
    GLuint texture;

    StaticMesh(GLuint vao, GLuint vbo_vertices, GLuint vbo_uvs, GLuint texture)
        : vao(vao), vbo_vertices(vbo_vertices), vbo_uvs(vbo_uvs), texture(texture) {}
};
