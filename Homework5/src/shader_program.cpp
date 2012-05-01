/*
 * shader_program.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 04/30/2012
 */
 
#include "shader_program.h"
#include <iostream>
#include <fstream>

ShaderProgram::ShaderProgram()
{
    _program = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    std::vector<GLuint>::iterator itr;
    for (itr = _shaders.begin(); itr != _shaders.end(); ++itr) {
        glDeleteShader(*itr);
    }
    
    glDeleteProgram(_program);
}


/* Loading Shaders */

bool ShaderProgram::loadShader(const char *filename, GLenum type)
{
    using namespace std;
    string shaderSource;
    
    // Read the shader source from the filename
    ifstream file(filename, ifstream::in);
    string line;
    while (file.good()) {
        getline(file, line);
        shaderSource += line;
    }
    
    // Create the shader
    GLuint shader = glCreateShader(type);
    
    // Load source and compile
    const char *shaderCStr = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderCStr, NULL);
    glCompileShader(shader);
    
    // Check if there's an error and log it
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint errLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errLength);
        
        char *errString = new char[errLength];
        glGetShaderInfoLog(shader, errLength, 0, errString);
        
        std::cerr << errString << std::endl;
        
        delete[] errString;
        return false;
    }
    
    // Attach and add to our array
    glAttachShader(_program, shader);
    _shaders.push_back(shader);
    
    return true;
}


/* Linking the Program */

bool ShaderProgram::linkProgram()
{
    glLinkProgram(_program);
    
    // Log the error if there is one
    GLint status;
    glGetProgramiv(_program, GL_LINK_STATUS, &status);
    if (!status) {
        // Print the error
        GLint errLength;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &errLength);
        
        char *errStr = new char[errLength];
        glGetProgramInfoLog(_program, errLength, 0, errStr);
        
        std::cerr << errStr << std::endl;
        
        delete[] errStr;
        return false;
    }
    
    return true;
}
