#include <cstdio>
#include <string.h>
#include <iostream>
#include <chrono> /*time features*/
#include <omp.h> /*using OpenMP parallel features*/
#include "filters.hpp"

/*
 * Using the "Cool image" library, available here:
 * http://cimg.eu/download.shtml
 * for os x: 
 * $brew install cimg
 * $brew install pkg-config cairo libpng jpeg giflib
 */

#ifndef cimg_use_jpeg
#define cimg_use_jpeg
#include "CImg.h"
#endif

#define USAGE "Usage:\t./image_manipulator input.jpg output.jpg filter\n\t./image_manipulator help"
#define AVAILABLE_FILTERS "/*\n *available filters:\n *\tgrayscale\n *\tblur\n *\trotate\n *\ttranspose\n *\tmirror\n */"

enum filter_num {
  NONE = -1,
  GRAYSCALE,
  BLUR,
  ROTATE,
  TRANSPOSE,
  MIRROR
};

inline filter_num filterCheck(char *filter){
  if(strcmp(filter, "grayscale") == 0)
    return GRAYSCALE;
  else if(strcmp(filter, "blur") == 0)
    return BLUR;
  else if(strcmp(filter, "rotate") == 0)
    return ROTATE;
  else if(strcmp(filter, "transpose") == 0)
    return TRANSPOSE;
  else if(strcmp(filter, "mirror") == 0)
    return MIRROR;
  else
    return NONE;
}

int main(int argc, char *argv[]){
  if(argc < 2 || argc > 4 || argc == 3){
    std::cout << USAGE << '\n';
    exit(0);
  }

  if(argc == 2){
    if(strcmp(argv[1], "help") == 0){
      std::cout << AVAILABLE_FILTERS << '\n';
    }else{
      std::cout << USAGE << '\n';
    }
    exit(0);
  }

  if(argc == 4){
    /*import image from jpg file*/
    cimg_library::CImg<unsigned char> input_img(argv[1]);

    /*create new image (width, height, depth, channels (RGB))*/
    cimg_library::CImg<unsigned char> output_img;

    /*using OpenMP to exploit parallelization*/
    omp_set_dynamic(0); /*turn off dynamic mode*/
    omp_set_num_threads(omp_get_num_procs()); /*get as many threads as there are processors*/

    /*take the overall time of execution*/
    std::chrono::system_clock::time_point overall_t1 = std::chrono::system_clock::now();

    filter_num x = filterCheck(argv[3]);
    if(x == NONE){
      std::cout << AVAILABLE_FILTERS << '\n';
      exit(0);
    }

    

    switch (x)
    {
      case GRAYSCALE:
        Filters::gray_scaler(input_img, output_img);
        break;
      case BLUR:
        Filters::blury(input_img, output_img);
        break;
      case ROTATE:
        Filters::rotator(input_img, output_img);
        break;
      case TRANSPOSE:
        Filters::transposer(input_img, output_img);
        break;
      case MIRROR:
        Filters::mirrored(input_img, output_img);
        break;
    }

    std::chrono::system_clock::time_point overall_t2 = std::chrono::system_clock::now();
    std::chrono::steady_clock::duration overall_duration = overall_t2-overall_t1;
    std::cout << "\noverall writing time: "<< std::chrono:: duration_cast<std::chrono::microseconds>(overall_duration).count()<<" microseconds\n\n";

    output_img.save_jpeg(argv[2]);
  }
  return 0;
}
