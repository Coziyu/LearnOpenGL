#pragma once
#include "headers/shader.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void reloadShader(GLFWwindow* window, Shader* target_shader);