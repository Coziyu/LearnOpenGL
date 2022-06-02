#include <iostream>
#include <fstream>
#include <streambuf>
#include <cmath>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "headers/func.h"
#include "headers/data.h"
//#include "headers/shader.h"
#include "../src/funcs/shader.cpp"
unsigned const int WIN_WIDTH = 800;
unsigned const int WIN_HEIGHT = 600; 

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH,WIN_HEIGHT,"Learning OpenGL",NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create OpenGL window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //This specifically resizes the window to the viewport once framebuffer_size_callback() gets called
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialze GLAD\n";
        return -1;
    }

    Shader myShader("./shaders/vertexShader1.vert","./shaders/fragmentShader1.frag");

    //Create and start VAO binding (Need to capture glBindBuffer(),glBufferData(),glVertexAttribPointer(),glEnableVertexAttribArray(),glUseProgram() )
    unsigned int VAO; //Tells opengl what data in VBO is and how it is formatted. 
    unsigned int VBO,VBO_Colour; //Stores vertices/vertex data to be passed into GPU for processing.
    unsigned int EBO;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_Colour);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); //<--- Start of VAO Bind
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//Buffer type of VBO is GL_ARRAY_BUFFER. 
    // glVertexAttribPointer will take data from currently binded VBO 
    // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Colour);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //! do not unbind EBO while VAO is active as the bound EBO is stored in the VAO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Setting up vertex attributes pointers
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO_Colour);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); //<--- End of VAO Bind

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        // inputA
        processInput(window);
        //rendering commands

        float timeValue = glfwGetTime();
        float modifier = (sin(3 * timeValue) / 2.0f) + 0.5f;
        
        myShader.use();
        glBindBuffer(GL_ARRAY_BUFFER,VBO); // Binded to update vertex pos
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        colours[0] = 1.0f - modifier;
        colours[4] = modifier;
        //colours[8] = 0.0f;
        glBindBuffer(GL_ARRAY_BUFFER,VBO_Colour);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_DYNAMIC_DRAW);

        glBindVertexArray(VAO); //Only because we unbinded VAO;
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);
        //check and call events and swap buffers

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0; 
}