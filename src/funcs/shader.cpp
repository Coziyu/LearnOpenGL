#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath){
        //* shaderSource is the actual text file where the shaders are written
        //* shaderCode is the pointer to the actual string literal in memory where it is accessed by openGL 
        //* 1) Getting shader source code from text files

        std::string vertexShaderSource,fragmentShaderSource;
        std::ifstream vertexShaderFile,fragmentShaderFile;

        //ensure ifstream objects can throw exceptions:
        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            //Open files
            vertexShaderFile.open(vertexPath);
            fragmentShaderFile.open(fragmentPath);
            std::stringstream vertexShaderStream,fragmentShaderStream;

            //Read file buffer contents into streams
            vertexShaderStream << vertexShaderFile.rdbuf();
            vertexShaderFile.close();
            fragmentShaderStream << fragmentShaderFile.rdbuf();
            fragmentShaderFile.close();

            //Conver SS into string
            vertexShaderSource = vertexShaderStream.str();
            fragmentShaderSource = fragmentShaderStream.str();
        }
        catch(const std::exception& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vertexShaderCode = vertexShaderSource.c_str();
        const char* fragmentShaderCode = fragmentShaderSource.c_str();

        //* 2)Compiling the shaders
        unsigned int vertexShader,fragmentShader;
        int success;
        char infoLog[512];

        //Compiling vertex shaders
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        //Compiling fragment shaders
        fragmentShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        //Creating shader program
        ID = glCreateProgram();
        glAttachShader(ID,vertexShader);
        glAttachShader(ID,fragmentShader);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    // use/activate the shader
    void use(){
        glUseProgram(ID);
    }
    // utility uniform functions
    void setBool(const std::string &name, bool value) const{         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value) const{ 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value) const{ 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    } 
};
  
