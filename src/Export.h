#ifndef __EXPORT_H__
#define __EXPORT_H__

#include "KnowdeFile.h"

class Export {
	public:
		static int exportKnowde(Knowde *knowde, char *target_file) { return 0; };
		static int exportFile(KnowdeFile *file, char *target_file) { return 0; };
};

#endif

