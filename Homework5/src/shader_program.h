/*
 * shader_program.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 04/30/2012
 */
 
#pragma once

#include "gl_includes.h"
#include <vector>

enum VertexAttribute {
    kVertexAttribPosition,
    kVertexAttribNormal,
    kVertexAttribColor,
    kVertexAttribTexCoord0,
    kVertexAttribTexCoord1,
    kVertexAttribCount
};

class ShaderProgram {
    GLuint _program;
    std::vector<GLuint> _shaders;
    
public:
    ShaderProgram();
    ~ShaderProgram();
    
    GLuint getProgram() const { return _program; }
    
    bool loadShader(const char *filename, GLenum type);
    bool linkProgram();
    
private:
    ShaderProgram(const ShaderProgram &o); // Non-copyable
    ShaderProgram& operator=(const ShaderProgram &o); // Non-copyable
};
