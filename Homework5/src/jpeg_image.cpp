/*
 * jpeg_image.cpp
 *
 * Author: Charles Magahern <charles.magahern@arizona.edu>
 * Date Created: 02/20/2012
 */
 
#include "jpeg_image.h"
#include <iostream>
#include <jpeglib.h>
#include <stdio.h>
#include <stdlib.h>
 
JPEGImage::JPEGImage(string filename) :
	_imageData(NULL)
{
	if (filename.size() > 0) {
		loadFile(filename);
	}
}

JPEGImage::~JPEGImage()
{
	if (_imageData != NULL) {
		_cleanupImageData();
	}
}

void JPEGImage::loadFile(string filename)
{
	const char *cfilename = filename.c_str();
	struct jpeg_decompress_struct comp_info;
	struct jpeg_error_mgr error;
	JSAMPROW row_ptr[1];
	unsigned char *img_data;
	unsigned data_counter = 0;

	if (_imageData != NULL) {
		_cleanupImageData();
	}
	
	FILE *file = fopen(cfilename, "rb");
	if (!file) {
		ImageException exc;
		exc.description = "Could not open JPEG file " + filename;
		throw exc;
	}

	comp_info.err = jpeg_std_error(&error);
	jpeg_create_decompress(&comp_info);
	jpeg_stdio_src(&comp_info, file);
	jpeg_read_header(&comp_info, true);

	jpeg_start_decompress(&comp_info);
	img_data   = (unsigned char *) malloc(comp_info.output_width * comp_info.output_height * comp_info.num_components);
	row_ptr[0] = (unsigned char *) malloc(comp_info.output_width * comp_info.num_components);

	while (comp_info.output_scanline < comp_info.image_height) {
		jpeg_read_scanlines(&comp_info, row_ptr, 1);
		for (unsigned i = 0; i < comp_info.image_width * comp_info.num_components; ++i) {
			img_data[data_counter++] = row_ptr[0][i];
		}
	}

	_imageData = img_data;
	_width = comp_info.output_width;
	_height = comp_info.output_height;
	_numComponents = comp_info.num_components;

	jpeg_finish_decompress(&comp_info);
	jpeg_destroy_decompress(&comp_info);
	free(row_ptr[0]);
	fclose(file);
}

unsigned char* JPEGImage::getImageData()
{
	return _imageData;
}


// Private Methods

void JPEGImage::_cleanupImageData()
{
	if (_imageData != NULL) {
		free(_imageData);
	}
	
	_width = 0;
	_height = 0;
	_numComponents = 0;
}