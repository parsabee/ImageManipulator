# image_manipulator

## image_manipulator_OpenMP

This version of the program uses the OpenMP parallel features to improve performance.
I've used the CImg library to import and export `.jpg` files, however teh filtering algorithms are my work
CImg library is available here:
```
http://cimg.eu/download.shtml
```
to install CImg library on OS X:
```
$brew install cimg
$brew install pkg-config cairo libpng jpeg giflib
```
I've used OpenMP's `parallel for` work sharing construct with a `collapse(2)`, to improve worksharing on nested loops between threads. I have provided a few `.jpg` images to work with. I take no credit for the images, I found them on the web.

## image_manipulator_serial

This version of the code is  an object oriented approach to manipulating `.pnm` files. This file format is used because it is very easy to write to and read from, and the purpose of this program is object orientation. I have provided a few `.pnm` files to work with. I take no credit for the images, I found them on the web.
