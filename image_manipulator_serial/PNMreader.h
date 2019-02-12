#ifndef _PNMREADER_H_
#define _PNMREADER_H_

#include "source.h"

class PNMreader : public Source {
	protected:
		char *filename;
	public:
		PNMreader(char *f);
		~PNMreader();
		virtual void Execute();
		virtual const char *SourceName();
};

#endif