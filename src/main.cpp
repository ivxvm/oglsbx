#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <entityx/entityx.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <functional>
#include <iostream>

#include "RenderContext.hpp"
#include "load_shaders.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using namespace entityx;

struct Transform {
    glm::vec3 position;
    glm::quat rotation;
};

struct PlayerInput {
    float axis_forward = 0;
    float axis_right = 0;
    float axis_up = 0;
    float mouse_delta_x = 0;
    float mouse_delta_y = 0;

    void clear_deltas() {
        this->mouse_delta_x = 0;
        this->mouse_delta_y = 0;
    }
};

struct Camera {
    struct {
        float sensitivity_x = 0.0075f;
        float sensitivity_y = 0.0075f;
    } config;

    float pitch = 0;
    float yaw = 0;

    glm::mat4 view;
};

struct CameraSystem : public System<CameraSystem> {
    void update(EntityManager &entityManager, EventManager &eventManager, TimeDelta delta) override {
        entityManager.each<Camera, Transform, PlayerInput>(
            [delta](Entity entity, Camera &camera, Transform &transform, PlayerInput &input) {
                camera.pitch += input.mouse_delta_y * camera.config.sensitivity_y;
                camera.pitch = min(+1.5708f, camera.pitch);
                camera.pitch = max(-1.5708f, camera.pitch);
                camera.yaw += input.mouse_delta_x * camera.config.sensitivity_x;
                transform.rotation =
                    glm::angleAxis(camera.pitch, glm::vec3(1, 0, 0)) *
                    glm::angleAxis(camera.yaw, glm::vec3(0, 1, 0));
                camera.view =
                    glm::mat4_cast(glm::normalize(transform.rotation)) *
                    glm::translate(glm::mat4(1.0f), -transform.position);
                auto forward = -glm::vec3(camera.view[0][2], camera.view[1][2], camera.view[2][2]);
                auto right = glm::vec3(camera.view[0][0], camera.view[1][0], camera.view[2][0]);
                auto up = glm::vec3(0, 1, 0);
                auto velocity = glm::normalize(glm::vec3(input.axis_forward, input.axis_right, 1));
                auto velocity_forward = velocity.x;
                auto velocity_right = velocity.y;
                transform.position +=
                    forward * velocity_forward * (float)delta +
                    right * velocity_right * (float)delta +
                    up * input.axis_up * (float)delta;
            });
    };
};

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
    RenderContext render_ctx;
    glBindVertexArray(render_ctx.vao);
    glBindBuffer(GL_ARRAY_BUFFER, render_ctx.vbo_cube_vertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, render_ctx.vbo_cube_uvs_alldiff);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);
    // ------------------------------------------------------------------------
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    int w = 0, h = 0, comp = 0;
    unsigned char *image = stbi_load("texture.png", &w, &h, &comp, STBI_rgb_alpha);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 96, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // ------------------------------------------------------------------------
    glm::mat4 proj_mat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 model_mat = glm::mat4(1.0f);
    // ------------------------------------------------------------------------
    glUseProgram(render_ctx.default_shader);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        double delta = glfwGetTime();
        glfwSetTime(0);
        glClearColor(0.1, 0.2, 0.4, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ex.systems.update<CameraSystem>(delta);
        glm::mat4 mvp = proj_mat * player_camera->view * model_mat;
        glUniformMatrix4fv(render_ctx.default_shader_mvp, 1, GL_FALSE, &mvp[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        player_input->clear_deltas();
        glfwPollEvents();
    }
    return 0;
}
