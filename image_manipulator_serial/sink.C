#include <sink.h>

Sink::Sink() {
	img1 = NULL;
	img2 = NULL;
}
 
void Sink::SetInput(const Image *i) {
	img1 = i;
}

void Sink::SetInput2(const Image *i2) {
	img2 = i2;
}
