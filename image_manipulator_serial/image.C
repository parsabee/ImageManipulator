#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "image.h"
#include "source.h"

void Image::Update() const {
	source->Update();
}

Image::Image(void) {
	height = 0;
	width = 0;
	buffer = NULL;
}

Image::Image(int h, int w, unsigned char *b) {
	height = h;
	width = w;
	buffer = (unsigned char *) malloc(3*height*width);
	memcpy(buffer, b, 3*height*width);
}

Image::Image(const Image &img) {
	buffer = (unsigned char *) malloc(3*height*width);
	height = img.height;
	width = img.width;
	memcpy(buffer, img.buffer, 3*height*width);
}

Image::~Image() {
	free(buffer);
}

void Image::ResetSize(int w, int h) {
	width = w;
	height = h;
}

void Image::setData(unsigned char *b) {
	if (buffer == NULL) {
		buffer = (unsigned char *) malloc(3*width*height);
	}
	memcpy(buffer, b, 3*height*width);
}

void Image::setSource(Source *s) {
	if (s != NULL) {
		source = s;
	}
}

int Image::getWidth() const {
	return width;
}

int Image::getHeight() const {
	return height;
}

unsigned char* Image::getData() const {
	return  buffer;
}