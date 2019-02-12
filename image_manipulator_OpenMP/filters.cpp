#include <omp.h>
#include <stdio.h>
#include "filters.hpp"

/* Assuming cache line size of 64 bytes
 * note: 
 *
 * no need for strip mining in the case of manipulating independent pixels
 * sustains overhead of creating and filling array when there is no need for it
 */

// void Filters::gray_scaler(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output)
// {
// 	int c, r; /*columns and rows*/
// 	const float divider = 1.0f/3.0f;
// 	const int vecLen = 4, float_cache_size = 16, width = input.width(), height = input.height();
//	output = cimg_library::CImg<unsigned char>(width, height, 1, 3);
// 	const int padded_height = height + float_cache_size - (height % float_cache_size);

// #pragma omp parallel for default(none) private(r) shared(input, output) collapse(2)
// 	for (c = 0; c < width; c++) {
// 		for (r = 0; r < padded_height; r += vecLen){

// 			float strip[vecLen][vecLen] __attribute__((aligned(64)));
// 		#pragma vector aligned
// 			int i;
// 			for (i = 0; i < vecLen; i++) {
// 				int new_r = r+i;
// 				if(new_r < height){
// 					// extract RGB values
// 					strip[0][i] = input(c, new_r, 0);
// 					strip[1][i] = input(c, new_r, 1);
// 					strip[2][i] = input(c, new_r, 2);
// 				}
// 			}
// 			for (i = 0; i < vecLen; i++){
// 				int new_r = r+i;
// 				if(new_r < height){
// 					// compute the average (grayscale)
// 					float avg = (strip[0][i] + strip[1][i] + strip[2][i]) * divider;
// 					// write to the output image
// 					output(c, r+i, 0) = output(c, r+i, 1) = output(c, r+i, 2) = avg; 
// 				}
// 			}
// 		}
// 	}
// }

/*Better performance*/
void Filters::gray_scaler(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output)
{
	int c, r; /*columns and rows*/
	const float divider = 1.0f/3.0f;
	const int width = input.width(), height = input.height();
	output = cimg_library::CImg<unsigned char>(width, height, 1, 3);

	#pragma omp parallel for default(none) private(c, r) shared(input, output) collapse(2)
	for (c = 0; c < width; c++) {
		for (r = 0; r < height; r++){
			unsigned char R, G, B;
			float avg;

			R = input(c, r, 0);
			G = input(c, r, 1);
			B = input(c, r, 2);

			avg = (float)(R+G+B) * divider;

			output(c, r, 0) = output(c, r, 1) = output(c ,r, 2) = avg;
		}
	}
}


void Filters::mirrored(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output)
{
	int c, r; /*columns and rows*/
	const int width = input.width(), height = input.height();
	output = cimg_library::CImg<unsigned char>(width, height, 1, 3);

#pragma omp parallel for default(none) private(r) shared(input, output)
	for (c = 0; c < width; c++) {
		for (r = 0; r < height; r++){
			int column = width - c - 1;

			output(column, r, 0) = input(c, r, 0);
			output(column, r, 1) = input(c, r, 1);
			output(column, r, 2) = input(c, r, 2);
		}
	}
}

void Filters::blury(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output)
{
	int c, r; /*columns and rows*/
	const int width = input.width(), height = input.height();
	output = cimg_library::CImg<unsigned char>(width, height, 1, 3);
	const float divider = 1.0f/81.0f;

#pragma omp parallel for default(none) private(c, r) shared(input, output) collapse(2)
	for (c = 0; c < width; c++) {
		for (r = 0; r < height; r++){
			float R = 0.0f, G = 0.0f, B = 0.0f;
			int x, y;
			if(c>=5 &&  r>=5 && c<=(width-5) && r<=(height-5)){
				// making every pixel blury by taking the average of its 120 neighboring pixels RGB
				for(x=0; x<9; x++){
					for(y=0; y<9; y++){
						R += (float)(input(c-4+x, r-4+y, 0));
						G += (float)(input(c-4+x, r-4+y, 1));
						B += (float)(input(c-4+x, r-4+y, 2));
					}
				}
				R *= divider;
				G *= divider;
				B *= divider;
				output(c, r, 0) = (unsigned char)R; 
				output(c, r, 1) = (unsigned char)G;
				output(c, r, 2) = (unsigned char)B;
			}else{
				output(c, r, 0) = input(c, r, 0); 
				output(c, r, 1) = input(c, r, 1);
				output(c, r, 2) = input(c, r, 2);
			}
		}
	}
}

void Filters::rotator(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output)
{
	int c, r; /*columns and rows*/
	const int width = input.width(), height = input.height();
	output = cimg_library::CImg<unsigned char>(height, width, 1, 3);
	
#pragma omp parallel for default(none) private(r) shared(input, output) collapse(2)
	for (c = 0; c < width; c++) {
		for (r = 0; r < height; r++){
			output(height - r, c, 0) = input(c, r, 0);
			output(height - r, c, 1) = input(c, r, 1);
			output(height - r, c, 2) = input(c, r, 2);
			
		}
	}
}

void Filters::transposer(const cimg_library::CImg<unsigned char> &input, cimg_library::CImg<unsigned char> &output)
{
	int c, r; /*columns and rows*/
	const int width = input.width(), height = input.height();
	output = cimg_library::CImg<unsigned char>(height, width, 1, 3);
	
#pragma omp parallel for default(none) private(r) shared(input, output) collapse(2)
	for (c = 0; c < width; c++) {
		for (r = 0; r < height; r++){
			output(r, c, 0) = input(c, r, 0);
			output(r, c, 1) = input(c, r, 1);
			output(r, c, 2) = input(c, r, 2);
		}
	}
}


