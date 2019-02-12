#ifndef _SOURCE_H_
#define _SOURCE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "image.h"

class Source {
	protected:
    	Image img;
    	virtual void Execute() = 0;
	public:
		Source();
		Image * GetOutput(void);
		virtual void Update();
		virtual const char *SourceName() = 0;
};

#endif