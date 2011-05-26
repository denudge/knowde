#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Widget.h"
#include "Knowde.h"
#include "Controller.h"
#include "Dater.h"
#include "Layouter.h"

class Editor : public Widget {
	public:
		Editor(Controller *_controller);
		virtual ~Editor();
		void show(Knowde *_knowde);
		void hide();
		void applyValues();

	protected:
		Knowde *knowde;
		GtkWidget *vbox;
		Dater dater;
		Layouter layouter;
		GtkAdjustment *hadj, *vadj;
		GtkTextBuffer *buffer;
		GtkWidget *notebook;
		GtkWidget *editor_label, *editor_box, *dater_label, *layouter_label;
		GtkWidget *title, *title_label;
		GtkWidget *descr, *descr_label;
		GtkWidget *keywords, *keywords_label;
		GtkWidget *label_box, *entry_box;
		GtkWidget *text_vbox, *layout_flag;
		GtkWidget *text, *text_label, *text_frame, *text_scroller;
		GtkWidget *header_box, *header_frame;
		GtkWidget *button_box, *ok_button, *apply_button, *cancel_button;
		gulong ok_handler, cancel_handler, delete_handler;
		
		void insert_text(GtkTextIter *iter);
		void update();

		static gboolean delete_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data);
		static gboolean ok_cb(GtkWidget *widget, gpointer data);
		static gboolean apply_cb(GtkWidget *widget, gpointer data);
		static gboolean cancel_cb(GtkWidget *widget, gpointer data);
		static gboolean insert_text_cb(GtkTextBuffer *buffer, GtkTextIter *pos, const gchar *text, gint length, gpointer data);
		static void layout_cb(GtkToggleButton *button, gpointer data);
};

#endif

