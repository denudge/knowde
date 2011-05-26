#ifndef __READER_H__
#define __READER_H_

#include "Widget.h"
#include "Controller.h"
#include "Knowde.h"

class Reader : public Widget {
	public:
		Reader(Controller *_controller);
		virtual ~Reader();
		void show(Knowde *knowde);
		void clear();
		void setKeywordTree(GtkTreeModel *tree_model);
		void displayHelp();

	protected:
		GtkTreeModel *keyword_tree;
		GtkWidget *title_label, *title, *ref_box;
		GtkWidget *descr_label, *descr, *date_label;
		GtkWidget *label_box, *entry_box;
		GtkWidget *header_frame, *header_box;
		GtkWidget *key_box, *key_choice, *key_jumper;
		GtkWidget *text_frame, *text, *text_scroller;
		
		void fillKeywordList(Knowde *knowde);
		void selectKnowde(Knowde *knowde);
		
		static void keyword_cb(GtkMenuItem *item, gpointer data);
};

#endif

