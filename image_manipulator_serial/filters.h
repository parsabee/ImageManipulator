#ifndef _FILTERS_H_
#define _FILTERS_H_
#include "source.h"
#include "sink.h"
#include "image.h"


class Filter : public Source, public Sink {
	public:
		virtual void Update();
		virtual const char *FilterName() = 0;
		virtual const char *SourceName();
		virtual const char *SinkName();
};

class Color : public Source {
	private:
		int w, h;
		unsigned char r, g, b;
	public:
		Color(int width, int height, unsigned char R, unsigned char G, unsigned char B);
		~Color();
		virtual void Execute();
		virtual const char *SourceName();
};

class CheckSum : public Sink {
	public:
		void OutputCheckSum(const char *filename);
		const char *SinkName() { return "CheckSum"; }
};

class Shrinker : public Filter {
	public:
    	virtual void Execute();
		virtual const char *FilterName();
};

class LRConcat : public Filter {
	public:
    	virtual void Execute();
		virtual const char *FilterName();
};

class TBConcat : public Filter {
	public:
    	virtual void Execute();
		virtual const char *FilterName();
};

class Blender : public Filter {
	protected:
    	float factor;
	public:
    	void SetFactor(float f);
    	virtual void Execute();
		virtual const char *FilterName();
};

class Mirror : public Filter {
	public:
		virtual void Execute();
		virtual const char *FilterName();
};

class Rotate : public Filter {
	public:
		virtual void Execute();
		virtual const char* FilterName();
};

class Subtract : public Filter {
	public:
		virtual void Execute();
		virtual const char* FilterName();
};

class Grayscale : public Filter {
	public:
		virtual void Execute();
		virtual const char *FilterName();
};

class Blur : public Filter {
	public:
		virtual void Execute();
		virtual const char *FilterName();
};
#endif