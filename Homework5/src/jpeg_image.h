/*
 * jpeg_image.h
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 02/20/2012
 */
 
#pragma once

#include <exception>
#include <string>

using std::string;

class ImageException : public std::exception {
public:
	string description;

	virtual const char* what() const throw()
	{
		string errorStr = "ImageException: " + description;
		return errorStr.c_str();
	}

	virtual ~ImageException() throw() {}
};

class JPEGImage {
	int _width;
	int _height;
	int _numComponents;
	unsigned char *_imageData;

public:
	JPEGImage(string filename = string());
	~JPEGImage();

	void loadFile(string filename);

	int getWidth() { return _width; }
	int getHeight() { return _height; }
	int getNumberOfComponents() { return _numComponents; }
	unsigned char* getImageData();

private:
	void _cleanupImageData();
};