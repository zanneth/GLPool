//
//  homework5.cpp
//  Homework5
//
//  Created by Charles Magahern on 4/20/12.
//  Copyright (c) 2012 omegaHern. All rights reserved.
//

#include "homework5.h"
#include "gl_includes.h"
#include "transform.h"
#include "vertex_data.h"

#include <ctime>
#include <iostream>

#define kImageWidth     1280.0f
#define kImageHeight    853.0f

#define kDefaultAmplitude       0.0f
#define kDefaultWaveVelocity    3.0f
#define kWaveVelocityIncrement  0.1f
#define kAmplitudeIncrement     0.1f

Homework5Application::Homework5Application(int argc, char **argv) : GraphicsApplication(argc, argv),
    _program(NULL),
    _projectionMatrixUniform(0),
    _modelMatrixUniform(0),
    _currentAmplitude(kDefaultAmplitude),
    _currentWaveVelocity(kDefaultWaveVelocity)
{
    setWindowTitle("Homework 5 - Charles Magahern");
    setTimerInterval(16); // 60 FPS
}

Homework5Application::~Homework5Application()
{
    if (_program != NULL) {
        delete _program;
    }
}


/* Callback Functions */

void Homework5Application::openGLReady()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    _mainCamera.position = Vecf(0.0, 0.0, 5.0);
    _mainCamera.look = Vecf(0.0, 0.0, 0.0);
    
    _setupGLEW();
    _setupShaders();
    _setupBuffers();
    _setupProjection();
    _setupImage();
}

void Homework5Application::displayCallback()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray(_vertexArray);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glutSwapBuffers();
}

void Homework5Application::timerFiredCallback()
{
    GLuint programID = _program->getProgram();
    glUseProgram(programID);
    
    
    // Update model-view matrix
    Transform modelTransform = Transform::identityTransform();
    modelTransform *= Transform::lookAt(_mainCamera.position, _mainCamera.look, Vecf(0.0, 1.0, 0.0));
    
    if (!_modelMatrixUniform) {
        _modelMatrixUniform = glGetUniformLocation(programID, "modelMatrix");
    }
    glUniformMatrix4fv(_modelMatrixUniform, 1, GL_FALSE, modelTransform.matrix);
    
    
    // Update time uniform
    glUniform1f(_timeUniform, _currentTime);
    _currentTime += 0.01;
    
    
    // Update amplitude uniform
    glUniform1f(_amplitudeUniform, _currentAmplitude);
    if (_currentAmplitude > 0.0) {
        _currentAmplitude -= 0.001;
    } else {
        _currentAmplitude = 0.0;
    }
    
    
    // Update wave velocity uniform
    glUniform1f(_waveVelocityUniform, _currentWaveVelocity);
    
    
    // Redraw
    glutPostRedisplay();
}

void Homework5Application::keyboardCallback(unsigned char key, int x, int y)
{
    switch (key) {
        case 'V':
            _currentWaveVelocity += kWaveVelocityIncrement;
            break;
        case 'v':
            _currentWaveVelocity -= kWaveVelocityIncrement;
            break;
        case 'A':
            _currentAmplitude += kAmplitudeIncrement;
            break;
        case 'a':
            _currentAmplitude -= kAmplitudeIncrement;
            if (_currentAmplitude < 0.0) {
                _currentAmplitude = 0.0;
            }
            break;
        case 'r':
        case 's':
            _currentWaveVelocity = kDefaultWaveVelocity;
            _currentAmplitude = kDefaultAmplitude;
            break;
        case 'd':
            _startNewWave();
            break;
        default:
            break;
    }
}


/* Private Functions */

void Homework5Application::_setupGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GLEW: " << glewGetErrorString(err) << std::endl;
        exit(err);
    }
}

void Homework5Application::_setupShaders()
{
    bool status = true;
    
    _program = new ShaderProgram();
    status &= _program->loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
    status &= _program->loadShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    
    GLuint programID = _program->getProgram();
    glBindAttribLocation(programID, kVertexAttribPosition, "position");
    glBindAttribLocation(programID, kVertexAttribTexCoord0, "texcoord0");
    
    _program->linkProgram();
    
    // Get uniforms
    _timeUniform = glGetUniformLocation(programID, "time");
    _amplitudeUniform = glGetUniformLocation(programID, "amplitude");
    _waveVelocityUniform = glGetUniformLocation(programID, "waveVelocity");
}

void Homework5Application::_setupBuffers()
{
    // Setup vertex arrays
    glGenVertexArrays(1, &_vertexArray);
    glBindVertexArray(_vertexArray);
    
    _setupPoolBuffers();
    _setupPhotoBuffers();
    
    glBindVertexArray(0);
}

void Homework5Application::_setupPoolBuffers()
{
#if 0
    // Create VBOs
    glGenBuffers(4, _poolVertexBuffers);
    
    // Setup position buffer
    glBindBuffer(GL_ARRAY_BUFFER, _poolVertexBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(poolVertexData), poolVertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(_vertexAttribLocations[kVertexAttribPosition], 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) NULL);
    glEnableVertexAttribArray(_vertexAttribLocations[kVertexAttribPosition]);
    
    // Setup color buffer
    glBindBuffer(GL_ARRAY_BUFFER, _poolVertexBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(poolColorData), poolColorData, GL_STATIC_DRAW);
    glVertexAttribPointer(_vertexAttribLocations[kVertexAttribColor], 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) NULL);
    glEnableVertexAttribArray(_vertexAttribLocations[kVertexAttribColor]);
    
    // Setup normals buffer
    glBindBuffer(GL_ARRAY_BUFFER, _poolVertexBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(poolNormalData), poolNormalData, GL_STATIC_DRAW);
    glVertexAttribPointer(_vertexAttribLocations[kVertexAttribNormal], 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) NULL);
    glEnableVertexAttribArray(_vertexAttribLocations[kVertexAttribNormal]);
    
    // Setup elements buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _poolVertexBuffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(poolElementData), poolElementData, GL_STATIC_DRAW);
#endif
}

void Homework5Application::_setupPhotoBuffers()
{
    // Create VBOs
    glGenBuffers(2, _photoVertexBuffers);
    
    // Setup position buffer
    glBindBuffer(GL_ARRAY_BUFFER, _photoVertexBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(photoVertexData), photoVertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(kVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) NULL);
    glEnableVertexAttribArray(kVertexAttribPosition);
    
    // Setup texcoord buffer
    glBindBuffer(GL_ARRAY_BUFFER, _photoVertexBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(photoTexCoordData), photoTexCoordData, GL_STATIC_DRAW);
    glVertexAttribPointer(kVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) NULL);
    glEnableVertexAttribArray(kVertexAttribTexCoord0);
}

void Homework5Application::_setupProjection()
{
    GLuint programID = _program->getProgram();
    
    glUseProgram(programID);
    
    Transform projection = Transform::perspective(45.0, _windowWidth / _windowHeight, 0.001, 10000.0);
    
    if (!_projectionMatrixUniform) {
        _projectionMatrixUniform = glGetUniformLocation(programID, "projMatrix");
    }
    glUniformMatrix4fv(_projectionMatrixUniform, 1, GL_FALSE, projection.matrix);
}

void Homework5Application::_setupImage()
{
    _picture.loadTextureJPEG("resources/goldengate.jpg");
    
    GLuint programID = _program->getProgram();
    glUseProgram(programID);
    
    GLint imageUniform = glGetUniformLocation(programID, "image");
    glUniform1i(imageUniform, 0);
}

void Homework5Application::_startNewWave()
{
    _currentWaveVelocity = kDefaultWaveVelocity;
    _currentAmplitude = 1.0;
}
