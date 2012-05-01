/*
 * picture.h
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 03/18/2012
 */
 
#pragma once

#include "geometry.h"
#include "gl_includes.h"
#include "jpeg_image.h"

class Picture {
    Rectf _frame;
    GLuint _texture;
    Colorf _shading;
    
public:
    Picture();
    
    // Accessors
    Rectf getFrame() const { return _frame; }
    GLuint getTexture() const { return _texture; }
    Colorf getShading() const { return _shading; }
    
    void setFrame(const Rectf &frame) { _frame = frame; }
    void setShading(const Colorf &shading) { _shading = shading; }
    
    // Loading Textures
    void loadTextureJPEG(const char *filename);
    
    // Drawing the Image using Texture Mapping
    void draw();
};
