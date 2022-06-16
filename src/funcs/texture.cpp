#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "headers/texture.h"
#include <glad/glad.h>

Texture2D::Texture2D(const char* texPath, GLint internalformat){

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int tex_width, tex_height, tex_nrChannels;
    //STBI reads image from up to down, while opengl reads image from down to up
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(texPath, &tex_width, &tex_height, &tex_nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, tex_width, tex_height, 0, internalformat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
        std::cout << "Path to failed texture: " << texPath << std::endl;
    }
    stbi_image_free(data);
}

void Texture2D::bind_texture(){
    glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture2D::configure_wrapping_s(GLint param){
    Texture2D::bind_texture();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,param);
}
void Texture2D::configure_wrapping_t(GLint param){
    Texture2D::bind_texture();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,param);
}
void Texture2D::configure_min_filter(GLint param){
    Texture2D::bind_texture();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,param);
}
void Texture2D::configure_mag_filter(GLint param){
    Texture2D::bind_texture();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,param);
}