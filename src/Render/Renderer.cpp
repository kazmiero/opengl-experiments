#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glu.h>

namespace /*anonymous*/
{
    void loadShaderCode(const std::string& filePath, std::string& code)
    {
        std::ifstream fileStream(filePath.c_str(), std::ios::in);
        if(fileStream.is_open()) {
            code.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
            fileStream.close();
        }
        else {
            std::cout << "Error opening file: " << filePath << std::endl;
        }
    }

    void loadAndCompileShader(const std::string& filePath, GLuint shaderID)
    {
        // Read the Vertex Shader code from the file
        std::string shaderCode;
        loadShaderCode(filePath, shaderCode);

        // Compile Shader
        std::cout << "Compiling shader : " << filePath << std::endl;
        const GLchar* shaderSource = shaderCode.c_str();
        glShaderSource(shaderID, 1, &shaderSource, NULL);
        glCompileShader(shaderID);
 
        GLint result = GL_FALSE;
        int infoLogLength;

        // Check Vertex Shader
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> shaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
        std::cout << shaderErrorMessage.data();
    }
    
    GLuint setupShaders(const std::string& vertexFilePath, const std::string& fragmentFilePath)
    { 
        // Create the shaders
        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
 
        loadAndCompileShader(vertexFilePath, vertexShaderID);
        loadAndCompileShader(fragmentFilePath, fragmentShaderID);
 
        // Link the program
        std::cout << "Linking program" << std::endl;
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        GLint result = GL_FALSE;
        int infoLogLength;
        
        // Check the program
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> programErrorMessage(infoLogLength);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        std::cout << programErrorMessage.data();
 
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
 
        return programID;
    }
}

Renderer::Renderer()
{}

void Renderer::init()
{
    vbo_ = {};
    
    vertices_ = {
        { {-1.0f, -1.0f, 0.0f} },
        { {1.0f, -1.0f, 0.0f} },
        { {0.0f, 1.0f, 0.0f} }
    };

    //vertices_ = {{{0.0f, 0.0f, 0.0f}}};
    
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(res) << std::endl;
    }

    GLuint programID = setupShaders("shader/shader.vs", "shader/shader.fs");
    glUseProgram(programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gluLookAt(0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size()*sizeof(std::array<float,3>), &vertices_[0], GL_STATIC_DRAW);
}

void Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Old school OpenGL equivalent
    /*glBegin(GL_TRIANGLES);
      glColor3f(1.0f, 0.0f, 0.0f);  
      glVertex3f(vertices_[0][0], vertices_[0][1], vertices_[0][2]);
      glVertex3f(vertices_[1][0], vertices_[1][1], vertices_[1][2]);
      glVertex3f(vertices_[2][0], vertices_[2][1], vertices_[2][2]);
      glEnd();*/

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}
