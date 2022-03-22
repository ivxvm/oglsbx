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
#include "entities/Player.hpp"
#include "systems/CameraSystem.hpp"
#include "systems/PhysicsSystem.hpp"
#include "systems/RenderStaticMeshesSystem.hpp"

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

void showFPS(GLFWwindow *pWindow) {
    static double delta = 0;
    static int nbFrames = 0;
    delta += glfwGetTime();
    nbFrames++;
    if (delta >= 1.0) {
        double fps = double(nbFrames) / delta;
        std::stringstream ss;
        ss << "oglsbx, fps: " << fps;
        glfwSetWindowTitle(pWindow, ss.str().c_str());
        delta = 0;
        nbFrames = 0;
    }
}

int main() {
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    auto window = glfwCreateWindow(1024, 768, "oglsbx", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    // ------------------------------------------------------------------------
    PhysicContext.initialize();
    RenderContext.initialize();
    EntityX ex;
    ex.systems.add<CameraSystem>();
    ex.systems.add<PhysicsSystem>();
    ex.systems.add<RenderStaticMeshesSystem>();
    ex.systems.configure();
    // ------------------------------------------------------------------------
    const Player PLAYER = Player();
    const GrassBlock GRASS_BLOCK = GrassBlock();
    // ------------------------------------------------------------------------
    for (int i = 0; i < 1000; i++) {
        GRASS_BLOCK.spawn_entity(
            ex,
            Transform(
                std::abs(std::rand()) % 20,
                std::abs(std::rand()) % 2,
                std::abs(std::rand()) % 20));
    }
    auto player = PLAYER.spawn_entity(ex, Transform(5, 5, 5));
    auto player_input = player.component<PlayerInput>().get();
    auto player_camera = player.component<Camera>().get();
    RenderContext.set_camera(player_camera);
    setup_input_handlers(window, player_input);
    glUseProgram(RenderContext.default_shader);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    while (!glfwWindowShouldClose(window)) {
        double delta = glfwGetTime();
        glfwSetTime(0);
        glClearColor(0.1, 0.2, 0.4, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ex.systems.update<PhysicsSystem>(delta);
        ex.systems.update<CameraSystem>(delta);
        ex.systems.update<RenderStaticMeshesSystem>(delta);
        glfwSwapBuffers(window);
        showFPS(window);
        player_input->clear_deltas();
        glfwPollEvents();
    }
    return 0;
}
