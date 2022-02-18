#pragma once

#include <OpenGL/gl3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint load_texture(const char *filename) {
    std::cout << "load_texture start" << std::endl;
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    std::cout << "load_texture after bind texture" << std::endl;
    int width = 0;
    int height = 0;
    int comp = 0;
    std::cout << "before stbi_load" << std::endl;
    unsigned char *image = stbi_load(filename, &width, &height, &comp, STBI_rgb_alpha);
    std::cout << "after stbi_load" << std::endl;
    std::cout << width << " " << height << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}
