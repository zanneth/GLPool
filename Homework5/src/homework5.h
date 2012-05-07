//
//  homework5.h
//  Homework5
//
//  Created by Charles Magahern on 4/20/12.
//  Copyright (c) 2012 omegaHern. All rights reserved.
//

#pragma once

#include "application.h"
#include "objects.h"
#include "picture.h"
#include "shader_program.h"
#include "transform.h"

class Homework5Application : public GraphicsApplication {
    ShaderProgram *_program;
    
    Picture _picture;
    Camera _mainCamera;
    
    GLuint _vertexArray;
    GLuint _photoVertexBuffers[2]; // For positions and texcoords
    
    GLfloat _currentTime;
    GLfloat _currentAmplitude;
    GLfloat _currentWaveVelocity;
    
    GLuint _modelMatrixUniform;
    GLuint _projectionMatrixUniform;
    GLuint _timeUniform;
    GLuint _amplitudeUniform;
    GLuint _waveVelocityUniform;
    
    Vec<int> _prevMouse;
    Vec<float> _currentRotation;
    
public:
    Homework5Application(int argc, char **argv);
    ~Homework5Application();
    
    void openGLReady();
    void displayCallback();
    void timerFiredCallback();
	void keyboardCallback(unsigned char key, int x, int y);
    void mouseCallback(int button, int state, int x, int y);
    void mouseMotionCallback(int x, int y);
    
private:
    void _setupGLEW();
    void _setupShaders();
    void _setupBuffers();
    void _setupPhotoBuffers();
    void _setupProjection();
    void _setupImage();
    
    void _startNewWave();
};
