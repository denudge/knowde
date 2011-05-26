#ifndef __SEARCHER_H__
#define __SEARCHER_H__

#include "Widget.h"

class Searcher : public Widget {
	public:
		Searcher(Controller* _controller);
		virtual ~Searcher();

		void show();
		void hide();

		char* getSearchExpression();
		int isTitle();
		int isDescription();
		int isText();
		int isKeywords();
		int isCaseSensitive();
		
	protected:
		Knowde* last_found;
		GtkWidget *entry, *entry_label;
		GtkWidget *option_frame, *option_box;
		GtkWidget *vbox;
		GtkWidget *option_case, *option_title, *option_descr, *option_text, *option_keywords;
		GtkWidget *ok_button, *cancel_button, *button_box;
		char expression[4096];

		void executeSearch();
		static gboolean ok_cb(GtkWidget *_widget, gpointer data);
		static gboolean cancel_cb(GtkWidget *_widget, gpointer data);
		static gboolean delete_event_cb(GtkWidget *_widget, GdkEvent *event, gpointer data);
};

#endif

