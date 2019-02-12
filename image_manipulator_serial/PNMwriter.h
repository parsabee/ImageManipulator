#ifndef _PNMWRITER_H_
#define _PNMWRITER_H_

#include "sink.h"

class PNMwriter : public Sink {
	public:
		void Write(char *filename);
		virtual const char *SinkName();
};

#endif
