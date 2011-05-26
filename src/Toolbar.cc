#ifndef __TOOLBAR_CC__
#define __TOOLBAR_CC__

#include "Toolbar.h"
#include "ToolbarXpm.h"

ButtonBox FileBar[] = {
	{ "New", 		CMD_FILE_NEW, 		GTK_STOCK_NEW, 		NULL, 	NULL },
	{ "Open",		CMD_FILE_OPEN, 	GTK_STOCK_OPEN, 		NULL,		NULL },
	{ "Save",		CMD_FILE_SAVE,		GTK_STOCK_SAVE,		NULL,		NULL },
	{ "Save As", 	CMD_FILE_SAVE_AS,	GTK_STOCK_SAVE_AS, 	NULL, 	NULL },
	{ "Close", 		CMD_FILE_CLOSE, 	GTK_STOCK_CLOSE, 		NULL, 	NULL },
	{ "Quit",		CMD_FILE_QUIT,		GTK_STOCK_QUIT,		NULL, 	NULL },
	{ "",				CMD_NONE, 			0,							NULL, 	NULL }
};

ButtonBox EditBar[] = {
	{ "Edit", 					CMD_EDIT_EDIT,						GTK_STOCK_PROPERTIES,	NULL, 			NULL },
	{ "New Top Knowde", 		CMD_EDIT_NEW_TOP_KNOWDE, 		NULL,							new_top_xpm,	NULL },
	{ "New Sibling Knowde",	CMD_EDIT_NEW_SIBLING_KNOWDE,	NULL,							new_sib_xpm,	NULL },
	{ "New Sub Knowde", 		CMD_EDIT_NEW_SUB_KNOWDE,		NULL,							new_sub_xpm,	NULL },
	{ "Copy", 					CMD_EDIT_COPY,						GTK_STOCK_COPY, 			NULL,				NULL },
	{ "Cut", 					CMD_EDIT_CUT,						GTK_STOCK_CUT, 			NULL,				NULL },
	{ "Paste", 					CMD_EDIT_PASTE,					GTK_STOCK_PASTE, 			NULL,				NULL },
	{ "",							CMD_NONE,							NULL,							NULL, 			NULL }
};

ButtonBox SearchBar[] = {
	{ "Toogle Tree View",		CMD_VIEW_TOGGLE_TREEVIEW,		NULL,	toggle_view_xpm, 	NULL },
	{ "Search",		CMD_SEARCH_SEARCH,		GTK_STOCK_FIND,	NULL, 	NULL },
	{ "Search Next", 	CMD_SEARCH_SEARCH_NEXT, 	GTK_STOCK_FIND, 	NULL, 	NULL },
	{ "", 				CMD_NONE, 					NULL, 				empty_xpm, 	NULL }
};

Toolbar::Toolbar(Controller *_controller) : Widget(_controller) {
	widget = gtk_hbox_new(FALSE,22);
	gtk_widget_show(widget);

	filebar = create_bar(FileBar);
	editbar = create_bar(EditBar);
	searchbar = create_bar(SearchBar);
	gtk_box_pack_start(GTK_BOX(widget),filebar,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(widget),editbar,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(widget),searchbar,FALSE,FALSE,0);
}

Toolbar::~Toolbar() {
}

GtkWidget* Toolbar::create_bar(ButtonBox *box) {
	int c = 0; GtkWidget *widget;

	widget = gtk_hbox_new(0,0);
	gtk_widget_show(widget);
	
	for (c=0;box[c].name[0];c++) {
		box[c].button = gtk_button_new();
		gtk_widget_show(box[c].button);
		g_object_set_data(G_OBJECT(box[c].button),"command",(gpointer) box[c].command);
		if (box[c].own_pix) {
			gtk_container_add(GTK_CONTAINER(box[c].button),gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_xpm_data(box[c].own_pix)));
		}
		else {
			gtk_container_add(GTK_CONTAINER(box[c].button),gtk_image_new_from_stock(box[c].stock_id,TOOLBAR_ICON_SIZE));
		}
		gtk_box_pack_start(GTK_BOX(widget),box[c].button,FALSE,FALSE,0);
		g_signal_connect(G_OBJECT(box[c].button),"clicked",G_CALLBACK(Toolbar::clicked_cb),this);
	}
	return widget;
}

void Toolbar::clicked(GtkWidget *widget) {
	int command = (int) g_object_get_data(G_OBJECT(widget),"command");
	if (controller) controller->executeCommand((Command) command);
}

gboolean Toolbar::clicked_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Toolbar*) data)->clicked(widget);
	return TRUE;
}

void Toolbar::setActivations(Activation* activations, ButtonBox *box) {
	int c;

	for (c=0;box[c].name[0];c++) {
		gtk_widget_set_sensitive(box[c].button,activations[(int) g_object_get_data(G_OBJECT(box[c].button),"command")]);
	}
}

void Toolbar::setActivations(Activation *activations) {
	setActivations(activations, FileBar);
	setActivations(activations, EditBar);
	setActivations(activations, SearchBar);
}

#endif

