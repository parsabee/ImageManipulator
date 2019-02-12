#include <iostream>
#include <fstream>
#include "PNMwriter.h"

using namespace std;

void PNMwriter::Write(char *filename) {
	int w = img1->getWidth();
	int h = img1->getHeight();
	ofstream ofs;

	ofs.open(filename, ios::binary);

	ofs << "P6" << endl;
	ofs << w << " " << h << endl;
	ofs << 255 << endl;

	ofs.write((char *) img1->getData(), w*h*3);

	ofs.close();
}

const char * PNMwriter::SinkName() {
	return "PNMwriter";
}