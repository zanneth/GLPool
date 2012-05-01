/*
 * application.cpp
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 02/20/2012
 */
 
#include "application.h"
#include "gl_includes.h"
#include <iostream>

static GraphicsApplication *currentApplication = NULL;
void displayCallback__internal();
void reshapeCallback__internal(int width, int height);
void keyboardCallback__internal(unsigned char key, int x, int y);
void specialKeyboardCallback__internal(int key, int x, int y);
void idleCallback__internal();
void timerFiredCallback__internal(int value);

GraphicsApplication::GraphicsApplication(int argc, char **argv) :
    _timerInterval(0),
    _windowWidth(WINDOW_WIDTH),
    _windowHeight(WINDOW_HEIGHT)
{
	setArguments(argc, argv);
}


/* Accessor Methods */

void GraphicsApplication::setArguments(int argc, char **argv)
{
	for (int i = 0; i < argc; ++i) {
		_arguments.push_back(argv[i]);
	}
}

list<string> GraphicsApplication::getArguments() const
{
	return _arguments;
}

void GraphicsApplication::setWindowTitle(const string &title)
{
	_windowTitle = title;
	glutSetWindowTitle(title.c_str());
}

string GraphicsApplication::getWindowTitle() const
{
	return _windowTitle;
}

void GraphicsApplication::setTimerInterval(const unsigned interval)
{
    if (_timerInterval == 0) {
        glutTimerFunc(interval, timerFiredCallback__internal, 0);
    }
    
    _timerInterval = interval;
}


/* Virtual Callbacks */

void GraphicsApplication::reshapeCallback(int width, int height)
{
    _windowWidth = width;
    _windowHeight = height;
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}


/* Initialization Methods */

void GraphicsApplication::_initializeGLUT(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	_windowHandle = glutCreateWindow(_windowTitle.c_str());
}

void GraphicsApplication::_initializeCallbacks()
{
	currentApplication = this;
	glutDisplayFunc(displayCallback__internal);
    glutReshapeFunc(reshapeCallback__internal);
	glutKeyboardFunc(keyboardCallback__internal);
    glutSpecialFunc(specialKeyboardCallback__internal);
	glutIdleFunc(idleCallback__internal);
}

void GraphicsApplication::_initializeOpenGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
}


/* Internal Callbacks */

void displayCallback__internal()
{
	currentApplication->displayCallback();
}

void reshapeCallback__internal(int width, int height)
{
    currentApplication->reshapeCallback(width, height);
}

void keyboardCallback__internal(unsigned char key, int x, int y)
{
	currentApplication->keyboardCallback(key, x, y);
}

void specialKeyboardCallback__internal(int key, int x, int y)
{
    currentApplication->specialKeyboardCallback(key, x, y);
}

void idleCallback__internal()
{
	currentApplication->idleCallback();
}

void timerFiredCallback__internal(int value)
{
    currentApplication->timerFiredCallback();
   
    unsigned currentInterval = currentApplication->getTimerInterval(); 
    if (currentInterval != 0) {
        glutTimerFunc(currentInterval, timerFiredCallback__internal, 0);
    }
}

/* Running the Application */

void GraphicsApplication::run()
{
	int argc = _arguments.size();
	char *argv[argc];
	list<string>::iterator itr;
	unsigned i;
	for (i = 0, itr = _arguments.begin(); itr != _arguments.end(); ++itr, ++i) {
		argv[i] = (char *) itr->c_str();
	}

	_initializeGLUT(argc, argv);
	_initializeCallbacks();
	_initializeOpenGL();

	openGLReady();

	glutMainLoop();
}