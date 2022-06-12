#pragma once
#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION // Will cause redefinition errors
#include "stb/stb_image.h"
#include <glad/glad.h>

class Texture2D
{
private:
    static unsigned int g_texture_unit_current_index;
public:
    unsigned int ID;

    Texture2D(const char* texPath, GLint internalformat); // * Constructor only constructs 2D Textures.

    void bind_texture();
    void configure_wrapping_s(GLint param);
    void configure_wrapping_t(GLint param);
    void configure_min_filter(GLint param);
    void configure_mag_filter(GLint param);
};