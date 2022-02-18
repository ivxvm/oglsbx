#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <entityx/entityx.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <functional>
#include <iostream>

#include "core/EntityDef.hpp"
#include "core/RenderContext.hpp"

#include "components/PlayerInput.hpp"
#include "components/Transform.hpp"
#include "entities/GrassBlock.hpp"
#include "systems/CameraSystem.hpp"

#include "utils/load_shaders.hpp"
#include "utils/load_texture.hpp"

using namespace std;
using namespace entityx;

void setup_input_handlers(GLFWwindow *window, PlayerInput *input) {
    assert(window != NULL);
    assert(input != NULL);
    static PlayerInput *input_ref;
    if (input_ref == NULL) {
        input_ref = input;
    } else {
        cout << "Error: setup_input_handlers called more than once!" << endl;
        exit(1);
    }
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        switch (action) {
        case GLFW_PRESS:
            switch (key) {
            case GLFW_KEY_W:
                input_ref->axis_forward += 1.0f;
                break;
            case GLFW_KEY_S:
                input_ref->axis_forward -= 1.0f;
                break;
            case GLFW_KEY_D:
                input_ref->axis_right += 1.0f;
                break;
            case GLFW_KEY_A:
                input_ref->axis_right -= 1.0f;
                break;
            case GLFW_KEY_SPACE:
                input_ref->axis_up += 1.0f;
                break;
            case GLFW_KEY_LEFT_SHIFT:
            case GLFW_KEY_RIGHT_SHIFT:
                input_ref->axis_up -= 1.0f;
                break;
            case GLFW_KEY_ESCAPE:
                if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                } else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                break;
            }
            break;
        case GLFW_RELEASE:
            switch (key) {
            case GLFW_KEY_W:
                input_ref->axis_forward -= 1.0f;
                break;
            case GLFW_KEY_S:
                input_ref->axis_forward += 1.0f;
                break;
            case GLFW_KEY_D:
                input_ref->axis_right -= 1.0f;
                break;
            case GLFW_KEY_A:
                input_ref->axis_right += 1.0f;
                break;
            case GLFW_KEY_SPACE:
                input_ref->axis_up -= 1.0f;
                break;
            case GLFW_KEY_LEFT_SHIFT:
            case GLFW_KEY_RIGHT_SHIFT:
                input_ref->axis_up += 1.0f;
                break;
            }
            break;
        }
    });
    static double prev_x;
    static double prev_y;
    glfwGetCursorPos(window, &prev_x, &prev_y);
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y) {
        input_ref->mouse_delta_x = x - prev_x;
        input_ref->mouse_delta_y = y - prev_y;
        prev_x = x;
        prev_y = y;
    });
}

int main() {
    EntityX ex;
    ex.systems.add<CameraSystem>();
    ex.systems.configure();
    auto player = ex.entities.create();
    player.assign<Transform>();
    player.assign<PlayerInput>();
    player.assign<Camera>();
    auto player_transform = player.component<Transform>().get();
    auto player_input = player.component<PlayerInput>().get();
    auto player_camera = player.component<Camera>().get();
    // ------------------------------------------------------------------------
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    auto window = glfwCreateWindow(1024, 768, "glfwtest", NULL, NULL);
    glfwMakeContextCurrent(window);
    setup_input_handlers(window, player_input);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    // --------------------------------
    RenderContext render_ctx(*player_camera);
    glBindVertexArray(render_ctx.vao);
    glBindBuffer(GL_ARRAY_BUFFER, render_ctx.vbo_cube_vertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, render_ctx.vbo_cube_uvs_alldiff);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);
    // ------------------------------------------------------------------------
    const GrassBlock GRASS_BLOCK = GrassBlock();
    // ------------------------------------------------------------------------
    render_ctx.bind_default_vao();
    // render_ctx.bind_cube_vertices_vbo();
    // render_ctx.bind_cube_alldiff_uvs_vbo();
    GLuint tex = load_texture("texture.png");
    glBindTexture(GL_TEXTURE_2D, tex);
    // ------------------------------------------------------------------------
    glm::mat4 proj_mat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 model_mat = glm::mat4(1.0f);
    // ------------------------------------------------------------------------
    glUseProgram(render_ctx.default_shader);
    glEnable(GL_DEPTH_TEST);
    for (int i = 0; i < 100; i++) {
        GRASS_BLOCK.spawn_entity(
            ex,
            Transform(
                std::abs(std::rand()) % 10,
                std::abs(std::rand()) % 2,
                std::abs(std::rand()) % 10));
    }
    // GRASS_BLOCK.spawn_entity(ex, Transform(0, 0, 0));
    // GRASS_BLOCK.spawn_entity(ex, Transform(1, 0, 0));
    // GRASS_BLOCK.spawn_entity(ex, Transform(2, 0, 0));
    // GRASS_BLOCK.spawn_entity(ex, Transform(0, 0, 1));
    // GRASS_BLOCK.spawn_entity(ex, Transform(1, 0, 1));
    // GRASS_BLOCK.spawn_entity(ex, Transform(2, 0, 1));
    while (!glfwWindowShouldClose(window)) {
        double delta = glfwGetTime();
        glfwSetTime(0);
        glClearColor(0.1, 0.2, 0.4, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ex.systems.update<CameraSystem>(delta);
        render_ctx.bind_default_vao();
        render_ctx.bind_cube_vertices_vbo();
        render_ctx.bind_cube_grasslike_uvs_vbo();
        glm::mat4 mvp = proj_mat * player_camera->view * model_mat;
        glUniformMatrix4fv(render_ctx.default_shader_mvp, 1, GL_FALSE, &mvp[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        GRASS_BLOCK.render(ex, render_ctx, delta);
        glfwSwapBuffers(window);
        player_input->clear_deltas();
        glfwPollEvents();
    }
    return 0;
}
