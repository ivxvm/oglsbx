#pragma once

#include <OpenGL/gl3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void check_shader_compile_errors(GLuint shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

GLuint load_shaders(const char *vertexPath, const char *fragmentPath) {
    std::string vert_code;
    std::string frag_code;
    std::ifstream vert_file;
    std::ifstream frag_file;
    vert_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vert_file.open(vertexPath);
        frag_file.open(fragmentPath);
        std::stringstream vert_stream;
        std::stringstream frag_stream;
        vert_stream << vert_file.rdbuf();
        frag_stream << frag_file.rdbuf();
        vert_file.close();
        frag_file.close();
        vert_code = vert_stream.str();
        frag_code = frag_stream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }
    const char *vShaderCode = vert_code.c_str();
    const char *fShaderCode = frag_code.c_str();
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vShaderCode, NULL);
    glCompileShader(vert);
    check_shader_compile_errors(vert, "VERTEX");
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fShaderCode, NULL);
    glCompileShader(frag);
    check_shader_compile_errors(frag, "FRAGMENT");
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    check_shader_compile_errors(program, "PROGRAM");
    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}
