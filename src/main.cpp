#include <iostream>
#include <fstream>
#include <streambuf>
#include <cmath>
#include <vector>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "headers/func.h"
#include "headers/data.h"
#include "headers/shader.h"
#include "headers/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


//TODO: IMPLEMENT CLASSES FOR DRAWING SPECIFIC ITEMS USING VAO, VBO, EBO
//TODO: IMPLEMENT CLASSES FOR TEXTURE LOADING AND USE
//TODO: Overload Shader::setMat() with other mat types when needed
//!ISSUE: Reloading shaders doesn't relocation uniform location. (Old shader ID gets deleted and not relooked up for the reloaded program ID); 

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
    unsigned int VBO_Vertex,VBO_Colour,VBO_TexCoords; //Stores vertices/vertex data to be passed into GPU for processing.
    unsigned int EBO;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO_Vertex);
    glGenBuffers(1, &VBO_Colour);
    glGenBuffers(1, &VBO_TexCoords);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); //<--- Start of VAO Bind
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertex);//Buffer type of VBO is GL_ARRAY_BUFFER. 
    // glVertexAttribPointer will take data from currently binded VBO 
    // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

    std::vector<float> &vertex_target = cube_vertices;

    glBufferData(GL_ARRAY_BUFFER, vertex_target.size() * sizeof(float), &vertex_target[0], GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Colour);
    glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(float), &colours[0], GL_STATIC_DRAW);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_TexCoords);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), &texCoords[0], GL_STATIC_DRAW);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //! do not unbind EBO while VAO is active as the bound EBO is stored in the VAO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0); //<--- End of VAO Bind

    //Generate textures

    Texture2D tex1("textures/container.jpg", GL_RGB);
    Texture2D tex2("textures/awesomeface.png", GL_RGBA);

    //tell opengl which texture is assigned to each sampler in shader code. (only needs to be done once)
    //unless shader program is reloaded. then bindings needs to be redone
    myShader.use();
    //glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);
    //can assign texture to uniform through shaderclass too
    myShader.setInt("texture1", 0);  //0 here points to the GL_TEXTURE0 texture unit
    glUniform1i(glGetUniformLocation(myShader.ID, "texture2"), 1);

    bool button_r_pressed = false; //For hot shader reloading


    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)){
        float time = glfwGetTime();
        
        // inputA
        processInput(window);
        if((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && !button_r_pressed){
            reloadShader(window, &myShader);
            myShader.use();
            myShader.setInt("texture1", 0);
            glUniform1i(glGetUniformLocation(myShader.ID, "texture2"), 1);
            button_r_pressed = true;
        }
        if(glfwGetKey(window,GLFW_KEY_R) == GLFW_RELEASE){
            button_r_pressed = false;
        }

        //rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        glActiveTexture(GL_TEXTURE0); //Using texture units 
        glBindTexture(GL_TEXTURE_2D, tex1.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex2.ID);
        
        myShader.use();
        glBindBuffer(GL_ARRAY_BUFFER,VBO_Vertex); // Binded to update vertex pos
        glBufferData(GL_ARRAY_BUFFER, vertex_target.size() * sizeof(float), &vertex_target[0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER,VBO_Colour);
        glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(float), &colours[0], GL_DYNAMIC_DRAW);

        myShader.setFloat("blend", blend);
        glBindVertexArray(VAO); //Only because we unbinded VAO;

        glm::mat4 transform(1.0f); //Identity matrix
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(myShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        myShader.setMat("projection", projection);



        int viewLocation = glGetUniformLocation(myShader.ID, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLocation = glGetUniformLocation(myShader.ID, "projection");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //check and call events and swap buffers

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0; 
}