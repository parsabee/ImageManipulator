#include "filters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

const char *Shrinker::FilterName() {
	return "Shrinker";
}
const char *LRConcat::FilterName() {
	return "LRConcat";
}
const char *TBConcat::FilterName() {
	return "TBConcat";
}
const char *Blender::FilterName() {
	return "Blender";
}
const char *Mirror::FilterName() {
	return "Mirror";
}
const char *Rotate::FilterName() {
	return "Rotate";
}
const char *Subtract::FilterName() {
	return "Subtract";
}
const char *Grayscale::FilterName() {
	return "Grayscale";
}
const char *Blur::FilterName() {
	return "Blur";
}
const char *Color::SourceName() {
	return "Color";
}
Color :: Color(int width, int height, unsigned char R, unsigned char G, unsigned char B){
	this->w = width;
	this->h = height;
	this->r = R;
	this->g = G;
	this->b = B;
}

Color :: ~Color() {}

void Color::Execute() {
	int i, j, in;
	this->img.ResetSize(this->w, this->h);
	unsigned char *temp = new unsigned char[this->w*this->h*3];
	for(i=0; i<this->w; i++){
		for(j=0; j<this->h; j++){
			in = (j*this->w+i)*3;
			temp[in]= this->r;
			temp[in+1]= this->g;
			temp[in+2]= this->b;
		}
	}
	img.setData(temp);
	delete [] temp;
}

void CheckSum::OutputCheckSum(const char *filename) {
	if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	int width = img1->getWidth();
	int height = img1->getHeight();
	int red = 0; 
	int green = 0; 
	int blue = 0;

	ofstream f;
	
	f.open(filename, ios::out);

	for(int i=0; i<width; i++) {
        for(int j=0; j<height; j++) {
            int in = (j*width+i)*3;
            red += img1->getData()[in];
            green += img1->getData()[in+1];
            blue += img1->getData()[in+2];
		
			red %= 256;
			green %= 256;
			blue %= 256;
        }
    } 

	f << "CHECKSUM: " << red << ", " << green << ", " << blue << endl;
	f.close();
}

void Filter::Update() {
	char msg[128];
	
	if(img1 != NULL) {
		sprintf(msg, "%s: about to update input1", FilterName());
		Logger::LogEvent(msg);
		img1->Update();
		sprintf(msg, "%s: done updating input1", FilterName());
		Logger::LogEvent(msg);
	}

	if(img2 != NULL) {
		sprintf(msg, "%s: about to update input2", FilterName());
		Logger::LogEvent(msg);
		img2->Update();
		sprintf(msg, "%s: done updating input2", FilterName());
		Logger::LogEvent(msg);
	}

	sprintf(msg, "%s: about to execute", FilterName());
	Logger::LogEvent(msg);
	Execute();	
	sprintf(msg, "%s: done executing", FilterName());
	Logger::LogEvent(msg);

}

const char *Filter::SourceName() {
	return FilterName();
}

const char *Filter::SinkName() {
	return FilterName();
}

void Mirror::Execute(){
	if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	int height = img1->getHeight();
	int width = img1->getWidth();
	img.ResetSize(width, height);
	unsigned char * temp =(unsigned char*) malloc(height*width*3);
	int i, j, in, out;
	for(i=0; i<width; i++){
		for(j=0; j<height; j++){
			in=(j*width+i)*3;
			out=(j*width+(width-1-i))*3;
			temp[in]= img1->getData()[out];
			temp[in+1]= img1->getData()[out+1];
			temp[in+2]= img1->getData()[out+2];
		}
	}
	img.setData(temp);
	free(temp);
}

void Rotate::Execute() {
	if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	int height = img1->getWidth();
	int width = img1->getHeight();
	img.ResetSize(width, height);
	unsigned char *temp = new unsigned char[width*height*3];
	int i, j, in, out;
	for(i=0; i<width; i++){
		for(j=0; j<height; j++){
			in = (i*height+j)*3;
			out = (j*width+(width-1-i))*3;
			temp[out]=img1->getData()[in]; 
			temp[out+1]=img1->getData()[in+1];
			temp[out+2]=img1->getData()[in+2];
		}
	}
	img.setData(temp);
	delete [] temp;
}

void Subtract::Execute() {
	if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if(img2 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input2!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if(img1->getWidth()!=img2->getWidth()){
		char msg[1024];
		sprintf(msg, "%s: input1 and input2 have different widths!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	if(img1->getHeight()!=img2->getHeight()){
		char msg[1024];
		sprintf(msg, "%s: input1 and input2 have different heights!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	int width = img1->getWidth();
	int height = img1->getHeight();
	img.ResetSize(width, height);
	unsigned char *temp = new unsigned char [width*height*3];
	int i, j, in, R, G, B;
	for(i=0; i<width; i++){
		for(j=0; j<height; j++){
			in = (j*width+i)*3;
			R = img1->getData()[in]-img2->getData()[in];
			G = img1->getData()[in+1]-img2->getData()[in+1];
			B = img1->getData()[in+2]-img2->getData()[in+2];
			if(R>=0)
				temp[in]=R;
			else
				temp[in]=0;
			if(G>=0)
				temp[in+1]=G;
			else
				temp[in+1]=0;
			if(B>=0)
				temp[in+2]=B;
			else
				temp[in+2]=0;
		}
	}
	img.setData(temp);
	delete [] temp;
}

void Grayscale::Execute() {
	if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	int width = img1->getWidth();
	int height = img1->getHeight();
	img.ResetSize(width, height);
	unsigned char *temp = new unsigned char[width*height*3];
	int i, j, in;
	for(i=0; i<width; i++){
		for(j=0; j< height; j++){
			in = (j*width+i)*3;
			temp[in] = img1->getData()[in]/5+img1->getData()[in+1]/2+img1->getData()[in+2]/4;
			temp[in+1] = img1->getData()[in]/5+img1->getData()[in+1]/2+img1->getData()[in+2]/4;
			temp[in+2] = img1->getData()[in]/5+img1->getData()[in+1]/2+img1->getData()[in+2]/4;
		}
	}
	img.setData(temp);
	delete [] temp;
}

void Blur::Execute(){
	if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	int width = img1->getWidth();
	int height = img1->getHeight();
	img.ResetSize(width, height);
	unsigned char *temp = new unsigned char[width*height*3];
	int i, j, in;
	for(i=0; i<width; i++){
		for(j=0; j< height; j++){
			in = (j*width+i)*3;
			if(i==0 || j==0 || i==(width-1) || j==(height-1)){
				temp[in] = img1->getData()[in];
				temp[in+1] = img1->getData()[in+1];
				temp[in+2] = img1->getData()[in+2];
			}
			else{
				temp[in] = img1->getData()[((j-1)*width+(i-1))*3]/8 + img1->getData()[((j-1)*width+i)*3]/8 
				+img1->getData()[((j-1)*width+(i+1))*3]/8 +img1->getData()[(j*width+(i-1))*3]/8+img1->getData()[(j*width+(i+1))*3]/8
				+img1->getData()[((j+1)*width+(i-1))*3]/8+img1->getData()[((j+1)*width+i)*3]/8+img1->getData()[((j+1)*width+(i+1))*3]/8;
				temp[in+1] = img1->getData()[(((j-1)*width+(i-1))*3)+1]/8 + img1->getData()[(((j-1)*width+i)*3)+1]/8
				+img1->getData()[(((j-1)*width+(i+1))*3)+1]/8 +img1->getData()[((j*width+(i-1))*3)+1]/8
				+img1->getData()[((j*width+(i+1))*3)+1]/8+img1->getData()[(((j+1)*width+(i-1))*3)+1]/8+img1->getData()[(((j+1)*width+i)*3)+1]/8
				+img1->getData()[(((j+1)*width+(i+1))*3)+1]/8;
				temp[in+2] = img1->getData()[(((j-1)*width+(i-1))*3)+2]/8 + img1->getData()[(((j-1)*width+i)*3)+2]/8
				+img1->getData()[(((j-1)*width+(i+1))*3)+2]/8 +img1->getData()[((j*width+(i-1))*3)+2]/8+img1->getData()[((j*width+(i+1))*3)+2]/8
				+img1->getData()[(((j+1)*width+(i-1))*3)+2]/8+img1->getData()[(((j+1)*width+i)*3)+2]/8+img1->getData()[(((j+1)*width+(i+1))*3)+2]/8;
			} 				
		}
	}
	img.setData(temp);
	delete [] temp;
}

void Shrinker::Execute() {

	if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	int width = img1->getWidth();
	int height = img1->getHeight();
	int halfWidth = width/2;
	int halfHeight = height/2;

	img.ResetSize(halfWidth, halfHeight);
    
	unsigned char * temp = (unsigned char *) malloc(halfWidth*halfHeight*3);
    
	for(int i=0; i<halfHeight; i++) {
		for(int j=0; j<halfWidth; j++) {
			int in = 3*(i*2*width+j*2);
			int out = 3*(i*halfWidth+j);
			temp[out] = img1->getData()[in];
			temp[out+1] = img1->getData()[in+1];
			temp[out+2] = img1->getData()[in+2];
		}
	}
    
	img.setData(temp);
	free(temp);
}

void LRConcat::Execute() {
    if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if(img2 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input2!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
	int width1 = img1->getWidth();
	int height1 = img1->getHeight();
	int width2 = img2->getWidth();
	int height2 = img2->getHeight();
	int outWidth = width1+width2;
	int outHeight = (height1+height2)/2;

   	if(height1 != height2) {
		char msg[1024];
		sprintf(msg, "%s: heights must match: %d, %d", SinkName(), height1, height2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	img.ResetSize(outWidth, outHeight);
	
	unsigned char * tempLR = (unsigned char *) malloc(outWidth*outHeight*3);
    
	for(int i=0; i < width1; i++) {
		for(int j=0; j < outHeight; j++) {
			int out = (j*outWidth+i)*3;
			int in = (j*width1+i)*3;
			tempLR[out] = img1->getData()[in];
			tempLR[out + 1] = img1->getData()[in + 1];
			tempLR[out + 2] = img1->getData()[in + 2];
		}
	}
	
	for(int i=0; i < width2; i++) {
		for(int j=0; j < outHeight; j++) {
			int in2 = (j*width2+i)*3;
			int out = (j*outWidth+width1+i)*3;
			tempLR[out] = img2->getData()[in2];
			tempLR[out + 1] = img2->getData()[in2 + 1];
			tempLR[out + 2] = img2->getData()[in2 + 2];
		}
	}
    
	img.setData(tempLR);
	
	free(tempLR);
}

void TBConcat::Execute() {
    if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if(img2 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input2!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	int width1 = img1->getWidth();
	int height1 = img1->getHeight();
	int width2 = img2->getWidth();
	int height2 = img2->getHeight();
	int outHeight = height1+height2;
	int outWidth = (width1+width2)/2;

    if(width1 != width2) {
		char msg[1024];
		sprintf(msg, "%s: widths must match: %d, %d", SinkName(), width1, width2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}
   
	img.ResetSize(outWidth, outHeight);
	
	unsigned char * tempTB = (unsigned char *) malloc(outHeight*outWidth*3);
    
	for(int i=0; i < outWidth; i++) {
		for(int j=0; j < height1; j++) {
			int out = (j*outWidth+i)*3;
			int in = (j*width1+i)*3;
			tempTB[out] = img1->getData()[in];
			tempTB[out + 1] = img1->getData()[in + 1];
			tempTB[out + 2] = img1->getData()[in + 2];
		}
	}
	
	for(int i=0; i < outWidth; i++) {
		for(int j=0; j < height2; j++) {
			int in2 = (j*width2+i)*3;
			int out = ((j+height1)*outWidth+i)*3;
			tempTB[out] = img2->getData()[in2];
			tempTB[out + 1] = img2->getData()[in2 + 1];
			tempTB[out + 2] = img2->getData()[in2 + 2];
		}
	}
    
	img.setData(tempTB);
	free(tempTB);
}

void Blender::SetFactor(float f) { 
	factor = f; 
}

void Blender::Execute() {
     if(img1 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input1!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	if(img2 == NULL) {
		char msg[1024];
		sprintf(msg, "%s: no input2!", SinkName());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	int width1 = img1->getWidth();
	int height1 = img1->getHeight();
	int width2 = img2->getWidth();
	int height2 = img2->getHeight();
    
	int outHeight = (height1+height2)/2;
	int outWidth = (width1+width2)/2;

    if(height1 != height2 || width1 != width2) {
		char msg[1024];
		sprintf(msg, "%s: image size must match: %d x %d, %d x %d", SinkName(), width1, height1, width2, height2);
		DataFlowException e(SinkName(), msg);
		throw e;
	}

    if(factor > 1.0) {
		char msg[1024];
		sprintf(msg, "%s: Invalid factor for Blender: %f", SinkName(), factor);
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	img.ResetSize(outWidth, outHeight);
    
	unsigned char * tempB = (unsigned char *) malloc(outHeight*outWidth*3);
    
	for(int i=0; i < outWidth; i++) {
		for(int j=0; j < outHeight; j++) {
			int index = (j*outWidth+i)*3;
			tempB[index] = img1->getData()[index]*factor + img2->getData()[index]*(1-factor);
			tempB[index + 1] = img1->getData()[index + 1]*factor + img2->getData()[index + 1]*(1-factor);
			tempB[index + 2] = img1->getData()[index + 2]*factor + img2->getData()[index + 2]*(1-factor);					
		}
	}
	
	img.setData(tempB);
	free(tempB);	
}