#ifndef _IMAGE_H_
#define _IMAGE_H_
#include "logging.h"

class Source;

class Image {
	private:
    	int height, width;
    	unsigned char *buffer;
		Source *source;
 
	public:
    	Image(void);
        Image(int height, int  weight, unsigned char *buffer);
        Image(const Image &img);
		~Image();

    	void ResetSize(int w, int h);
    	void setData(unsigned char *b);
		void setSource(Source *s);
    	int getWidth() const;
    	int getHeight() const;
    	unsigned char* getData() const;
		void Update() const;
};

#endif