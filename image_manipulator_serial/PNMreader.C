#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "PNMreader.h"

using namespace std;

PNMreader::PNMreader(char *f) {
	filename = (char *) malloc(strlen(f)+1);
	strcpy(filename, f);
}

PNMreader::~PNMreader() {
	free(filename);
}

const char *PNMreader::SourceName() {
	return "PNMreader";
}

void PNMreader::Execute() {

    ifstream ifs;
	ifs.open(filename, ios::binary);

    char magicNum[128];
	int width, height, maxval;
	
	if(!ifs) {
		cerr <<  "Unable to open file " << filename << endl;
		free(filename);
		exit(0);
	}

	ifs >> magicNum >> width >> height >> maxval;
	ifs.get();

	img.ResetSize(width, height);

 
	if(strcmp(magicNum, "P6") != 0) {
		cerr <<  "Unable to read from file " << filename << ", because it is not a PNM file of type P6" << endl;
		free(filename);
		exit(0);
	}
	
	unsigned char * temp = (unsigned char *) malloc(3*height*width);

	ifs.read((char *) temp, width*height*3); 
	img.setData((unsigned char *) temp);

	ifs.close();
	free(temp);
}