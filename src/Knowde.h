#ifndef __KNOWDE_H__
#define __KNOWDE_H__

#include <glib.h>
#include <vector>
#include "DatePoint.h"
#include <gdk/gdk.h>

typedef struct {
	GString *text_font;
		
} Layout;

class Knowde {
	public:
		Knowde(Knowde* _parent = NULL, char* hash = NULL);
		virtual ~Knowde();
		
		char* getHash();
		char* getTitle();
		char* getDescr();
		char* getKeywords();
		char* getText();
		unsigned long getTextSize();
		void setTitle(char* _title);
		void setDescr(char* _descr);
		void setKeywords(char *word);
		void setText(char* _text);
		Knowde* getParent();
		Knowde* newChild(char* hash = NULL);
		unsigned long getSize();
		unsigned long getSizeRecoursive();
		void getPathString(char *dest);
		void print();
		
		void adoptChild(Knowde *child);
		void unparent();
		void reparent(Knowde *newParent);
		Knowde* cutChild(unsigned int child_index);

		int countChildren();
		Knowde* getChild(int index);
		Knowde* clone();
		void removeChild(int index);

		// date functionality
		void enableDate();
		void disableDate();
		DatePoint* getStartDate();
		DatePoint* getEndDate();
		int hasDate();
		char* getDate();
		int isDateDurative();
		void setDateDurative(int yesno);
		void normalizeDate();

		// layout functionality
		int hasTextFont();
		void disableTextFont();
		void setTextFont(char *font);
		char* getTextFont();
		int hasTextColor();
		void disableTextColor();
		void setTextColor(GdkColor *color);
		GdkColor* getTextColor();
		int hasBackgroundColor();
		void disableBackgroundColor();
		void setBackgroundColor(GdkColor *color);
		GdkColor *getBackgroundColor();

	protected:
		char 			hash[64];
		GString* 	title;
		GString* 	descr;
		GString* 	text;
		GString*		keywords;
		GString*		date;
		Knowde* 		parent;
		DatePoint	*start_date;
		DatePoint 	*end_date;
		GString*		text_font;
		GdkColor*	text_color;
		GdkColor*	bg_color;
		std::vector<Knowde*> children;
};

#endif

