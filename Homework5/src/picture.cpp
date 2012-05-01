/*
 * picture.cpp
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 03/18/2012
 */
 
#include "picture.h"

Picture::Picture() :
    _texture(0),
    _shading(Colorf(1.0, 1.0, 1.0))
{}

void Picture::loadTextureJPEG(const char *filename)
{
    if (filename == NULL) {
        return;
    }
    
	JPEGImage image;
    image.loadFile(filename);
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    unsigned char *imageData = image.getImageData();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 image.getWidth(), image.getHeight(),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 imageData);
    _texture = texture;
}

void Picture::draw()
{
    glColor3f(_shading.red, _shading.green, _shading.blue);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 1.0);
    glVertex3f(_frame.origin.x, _frame.origin.y, _frame.origin.z);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(_frame.origin.x + _frame.size.x, _frame.origin.y, _frame.origin.z);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(_frame.origin.x + _frame.size.x, _frame.origin.y + _frame.size.y, _frame.origin.z);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(_frame.origin.x, _frame.origin.y + _frame.size.y, _frame.origin.z);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
