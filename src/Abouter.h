#ifndef __ABOUTER_H__
#define __ABOUTER_H__

#include "config.h"
#include "Widget.h"

class Abouter : public Widget {
	public:
		Abouter(Controller *_controller);
		virtual ~Abouter();
		void hide();
		void show();

	protected:
		GtkWidget *vbox;
		GtkWidget *program_label, *version_label;
		GtkWidget *author_label, *email_label;
		GtkWidget *copyright_label, *date_label;
		GtkWidget *gnu_label;
		GtkWidget *ok_button;

		static gboolean delete_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data);
		static gboolean ok_cb(GtkWidget *widget, gpointer data);
};

#endif

