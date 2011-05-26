#ifndef __KNOWDE_FILE_H__
#define __KNOWDE_FILE_H__

#include <glib.h>
#include <stdio.h>
#include "Knowde.h"
#include <libxml/parser.h>

class KnowdeFile {
	public:
		KnowdeFile(char *_filename = NULL);
		virtual ~KnowdeFile();
		char* getFilename();
		void setFilename(char *_filename);
		Knowde* getData();
		void removeKnowde(Knowde *knowde);
		void save();
		void saveAs(char *_filename);
		void load();
		
		int hasDefaultTextFont();
		char* getDefaultTextFont();
		void disableDefaultTextFont();
		void setDefaultTextFont(char* font);

	protected:
		void removeKnowde(Knowde *knowde, Knowde *start);
		void saveXMLString(char *text, int len, FILE *file);
		void saveKnowde(Knowde *knowde, FILE *file, int indent);
		void loadXMLNode(xmlNode *node, Knowde* _parent);
		GString *filename;
		Knowde *data;

};

#endif

