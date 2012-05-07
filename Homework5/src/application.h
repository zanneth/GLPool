/*
 * application.h
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 02/20/2012
 */
 
#pragma once

#include <string>
#include <list>

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768
 
using std::list;
using std::string;

class GraphicsApplication {
protected:
	list<string> _arguments;
	string _windowTitle;
	unsigned _windowHandle;
    unsigned _timerInterval;
    int _windowWidth, _windowHeight;

public:
	GraphicsApplication(int argc, char **argv);
	virtual ~GraphicsApplication() {}

	void setArguments(int arg, char **argv);
	list<string> getArguments() const;
	void setWindowTitle(const string &title);
	string getWindowTitle() const;
    void setTimerInterval(const unsigned interval);
    unsigned getTimerInterval() const { return _timerInterval; }

	virtual void run();
	
	virtual void displayCallback() {}
    virtual void reshapeCallback(int width, int height);
	virtual void keyboardCallback(unsigned char key, int x, int y) {}
    virtual void specialKeyboardCallback(int key, int x, int y) {}
    virtual void mouseCallback(int button, int state, int x, int y) {}
    virtual void mouseMotionCallback(int x, int y) {}
	virtual void idleCallback() {}
    virtual void timerFiredCallback() {}
	virtual void openGLReady() {}

private:
	void _initializeGLUT(int argc, char **argv);
	void _initializeCallbacks();
	void _initializeOpenGL();
};