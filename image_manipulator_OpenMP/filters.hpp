#ifndef FILTERS_H
#define FILTERS_H

#ifndef cimg_use_jpeg
#define cimg_use_jpeg
#include "CImg.h"
#endif

namespace Filters{

	void gray_scaler(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output);
	void mirrored(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output);
	void blury(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output);
	void rotator(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output);
	void transposer(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output);
}

#endif