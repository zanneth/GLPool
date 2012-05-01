/*
 * gl_includes.h
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 02/20/2012
 */
 
#pragma once

#ifdef __APPLE__
#	include <GL/glew.h>
#	include <OpenGL/glu.h>
#	include <GLUT/glut.h>
#else
#	include <GL/glew.h>
#	include <GL/glu.h>
#	include <GL/glut.h>
#endif