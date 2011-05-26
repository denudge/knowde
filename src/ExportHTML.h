#ifndef __EXPORT_HTML_H__
#define __EXPORT_HTML_H__

#include "Export.h"

class ExportHTML : public Export {
	public:
		static int exportKnowde(Knowde *knowde, char *target_file);
		static int exportFile(KnowdeFile *file, char *target_file);

	protected:
		static void saveXMLString(char *text, int len, FILE *file);	
};

#endif

