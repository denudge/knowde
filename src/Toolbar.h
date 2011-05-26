#ifndef __TOOLBAR_H__
#define __TOOLBAR_H__

#include "Widget.h"
#include "Controller.h"

typedef struct _ButtonBox ButtonBox;
struct _ButtonBox {
	char *name;
	Command command;
	gchar *stock_id;
	const char **own_pix;
	GtkWidget *button;
};

class Toolbar : public Widget {
	public:
		Toolbar(Controller *_controller);
		virtual ~Toolbar();
		void setActivations(Activation *activations);

	protected:
		GtkWidget *filebar, *editbar, *searchbar;
		GtkWidget* create_bar(ButtonBox *box);
		void setActivations(Activation *activations, ButtonBox *box);
		static gboolean clicked_cb(GtkWidget *widget, gpointer data);
		void clicked(GtkWidget *widget);
};

#endif

