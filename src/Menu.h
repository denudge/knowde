#ifndef __MENU_H__
#define __MENU_H__

#include "Widget.h"
#include "Controller.h"

typedef struct _MenuBox MenuBox;
struct _MenuBox {
	char *name;
	char *accel;
	gchar *stock_id;
	const char **own_pix;
	int command;
	GtkWidget *menu;
	GtkWidget *entry;
	MenuBox* items;
};

class Menu : public Widget {
	public:
		Menu(Controller *_controller);
		virtual ~Menu();
		void setActivations(Activation *activations);
		GtkWidget* getPopupMenu();

	protected:
		GtkWidget *popup_menu;
		void setActivations(Activation *activations, MenuBox *box);
		static void activate_cb(GtkMenuItem *item, gpointer data);
		void activate(GtkMenuItem *item);
		GtkWidget* create_menu(MenuBox *box = NULL);
		GtkWidget* create_context_menu(MenuBox *box = NULL);
};

#endif


