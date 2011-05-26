#ifndef __MENU_CC__
#define __MENU_CC__

#include <glib.h>
#include <stdio.h>
#include "Menu.h"
#include "ToolbarXpm.h"

MenuBox FileMenuBox[] = {
	{ _("New"), "Ctrl-N", GTK_STOCK_NEW, NULL,			CMD_FILE_NEW, 		NULL, NULL, NULL },
	{ _("Open"), "Ctrl-O", GTK_STOCK_OPEN, NULL,			CMD_FILE_OPEN,    NULL, NULL, NULL },
	{ _("Save"), "Ctrl-S", GTK_STOCK_SAVE, NULL,			CMD_FILE_SAVE,    NULL, NULL, NULL },
	{ _("Save As"), "", GTK_STOCK_SAVE_AS, NULL,			CMD_FILE_SAVE_AS, NULL, NULL, NULL },
	{ _("Close"), "Ctrl-W",	GTK_STOCK_CLOSE, NULL,		CMD_FILE_CLOSE, 	NULL, NULL, NULL },
	{ _("Quit"), "Ctrl-Q", GTK_STOCK_QUIT, NULL,			CMD_FILE_QUIT, 	NULL, NULL, NULL },
	{ "",NULL, NULL, NULL,					0, 					NULL, NULL, NULL }
}; 
 
MenuBox ContextFileMenuBox[] = {
	{ _("New"),"Ctrl-N", GTK_STOCK_NEW, NULL,			CMD_FILE_NEW, 		NULL, NULL, NULL },
	{ _("Open"),"Ctrl-O", GTK_STOCK_OPEN, NULL,			CMD_FILE_OPEN,    NULL, NULL, NULL },
	{ _("Save"),"Ctrl-S", GTK_STOCK_SAVE, NULL,			CMD_FILE_SAVE,    NULL, NULL, NULL },
	{ _("Save As"),"", GTK_STOCK_SAVE_AS, NULL,		CMD_FILE_SAVE_AS, NULL, NULL, NULL },
	{ _("Close"),"Ctrl-W", GTK_STOCK_CLOSE, NULL, 			CMD_FILE_CLOSE, 	NULL, NULL, NULL },
	{ _("Quit"),"Ctrl-Q", GTK_STOCK_QUIT, NULL,			CMD_FILE_QUIT, 	NULL, NULL, NULL },
	{ "",NULL, NULL, NULL,					0, 					NULL, NULL, NULL }
};

MenuBox EditMenuBox[] = {
	{ _("Edit"),"Ctrl-E", GTK_STOCK_PROPERTIES, NULL,							CMD_EDIT_EDIT, 						NULL, NULL, NULL },
	{ _("New Top Knowde"),"", NULL, new_top_xpm,				CMD_EDIT_NEW_TOP_KNOWDE, 			NULL, NULL, NULL },
	{ _("New Sibling Knowde"),"",	NULL, new_sib_xpm,		CMD_EDIT_NEW_SIBLING_KNOWDE,		NULL, NULL, NULL },
	{ _("New Sub Knowde"),"", NULL, new_sub_xpm, 				CMD_EDIT_NEW_SUB_KNOWDE,			NULL, NULL, NULL },
	{ _("Copy"),"Ctrl-C", GTK_STOCK_COPY, NULL,                 	CMD_EDIT_COPY, 						NULL, NULL, NULL },
	{ _("Cut"),"Ctrl-X", GTK_STOCK_CUT, NULL,                  	CMD_EDIT_CUT, 							NULL, NULL, NULL },
	{ _("Paste"),"Ctrl-V", GTK_STOCK_PASTE, NULL,                	CMD_EDIT_PASTE, 						NULL, NULL, NULL },
	{ _("Clear Clipboard"),"", NULL, empty_xpm,       	CMD_EDIT_CLEAR_CLIPBOARD, 			NULL, NULL, NULL },
	{ "",NULL, NULL, NULL,										0, 										NULL, NULL, NULL }
};

MenuBox ContextEditMenuBox[] = {
	{ "Edit","Ctrl-E", GTK_STOCK_PROPERTIES, NULL, 							CMD_EDIT_EDIT, 						NULL, NULL, NULL },
	{ "New Top Knowde","", NULL, new_top_xpm, 			CMD_EDIT_NEW_TOP_KNOWDE, 			NULL, NULL, NULL },
	{ "New Sibling Knowde","",	NULL, new_sib_xpm, 		CMD_EDIT_NEW_SIBLING_KNOWDE,		NULL, NULL, NULL },
	{ "New Sub Knowde","", NULL, new_sub_xpm, 			CMD_EDIT_NEW_SUB_KNOWDE,			NULL, NULL, NULL },
	{ "Copy","Ctrl-C", GTK_STOCK_COPY, NULL,                  	CMD_EDIT_COPY, 						NULL, NULL, NULL },
	{ "Cut","Ctrl-X", GTK_STOCK_CUT, NULL,                   	CMD_EDIT_CUT, 							NULL, NULL, NULL },
	{ "Paste","Ctrl-V", GTK_STOCK_PASTE, NULL,                 	CMD_EDIT_PASTE, 						NULL, NULL, NULL },
	{ "Clear Clipboard","", NULL, empty_xpm,       	CMD_EDIT_CLEAR_CLIPBOARD, 			NULL, NULL, NULL },
	{ "",NULL, NULL, NULL,		  							0, 										NULL, NULL, NULL }
};

MenuBox ViewMenuBox[] = {
	{ "Toggle Tree View","Ctrl-T", NULL, toggle_view_xpm, CMD_VIEW_TOGGLE_TREEVIEW, NULL, NULL, NULL },
	{ "",NULL, NULL, NULL,		  				0, 							NULL, NULL, NULL }
};

MenuBox ContextViewMenuBox[] = {
	{ "Toggle Tree View","Ctrl-T", NULL, toggle_view_xpm, CMD_VIEW_TOGGLE_TREEVIEW, NULL, NULL, NULL },
	{ "",NULL, NULL, NULL, 		  				0, 							NULL, NULL, NULL }
};

MenuBox SearchMenuBox[] = {
	{ "Search","Ctrl-F", GTK_STOCK_FIND, NULL, 		CMD_SEARCH_SEARCH, 		NULL, NULL, NULL },
	{ "Search Next","Ctrl-G", GTK_STOCK_FIND, NULL,	CMD_SEARCH_SEARCH_NEXT,	NULL, NULL, NULL },
	{ "",NULL, NULL, NULL, 				0,								NULL, NULL, NULL }
};

MenuBox ContextSearchMenuBox[] = {
	{ "Search","Ctrl-F", GTK_STOCK_FIND, NULL, 		CMD_SEARCH_SEARCH, 		NULL, NULL, NULL },
	{ "Search Next","Ctrl-G", GTK_STOCK_FIND, NULL, 	CMD_SEARCH_SEARCH_NEXT, NULL, NULL, NULL },
	{ "",NULL, NULL, NULL, 				0,								NULL, NULL, NULL }
};

MenuBox ExportMenuBox[] = {
	{ "File as HTML",NULL, NULL, toggle_view_xpm, CMD_EXPORT_FILE_AS_HTML, NULL, NULL, NULL },
	{ "Knowde as HTML","Ctrl-H", NULL, toggle_view_xpm, CMD_EXPORT_KNOWDE_AS_HTML, NULL, NULL, NULL },
	{ "",NULL, NULL, NULL,		  				0, 							NULL, NULL, NULL }
};

MenuBox ContextExportMenuBox[] = {
	{ "File as HTML",NULL, NULL, toggle_view_xpm, CMD_EXPORT_FILE_AS_HTML, NULL, NULL, NULL },
	{ "Knowde as HTML","Ctrl-H", NULL, toggle_view_xpm, CMD_EXPORT_KNOWDE_AS_HTML, NULL, NULL, NULL },
	{ "",NULL, NULL, NULL,		  				0, 							NULL, NULL, NULL }
};

MenuBox HelpMenuBox[] = {
	{ "About","Ctrl-I", NULL, empty_xpm,	CMD_HELP_ABOUT, 	NULL, NULL, NULL },
	{ "Help","", GTK_STOCK_HELP, 	NULL, CMD_HELP_HELP, 	NULL, NULL, NULL },
	{ "",NULL, NULL, NULL,		0, 					NULL, NULL,	NULL }
};

MenuBox ContextHelpMenuBox[] = {
	{ "About","Ctrl-I", NULL, empty_xpm,	CMD_HELP_ABOUT, 	NULL, NULL, NULL },
	{ "Help","", GTK_STOCK_HELP,	NULL, CMD_HELP_HELP, 	NULL, NULL, NULL },
	{ "",NULL, NULL, NULL,		0, 					NULL, NULL,	NULL }
};

MenuBox MainMenuBox[] = {
	{ "File", NULL, NULL, NULL,		0, NULL, NULL, FileMenuBox },
	{ "Edit",NULL, NULL, NULL, 		0, NULL, NULL, EditMenuBox },
	{ "View",NULL, NULL, NULL, 		0, NULL, NULL, ViewMenuBox },
	{ "Search",NULL, NULL, NULL,  	0, NULL, NULL, SearchMenuBox },
	{ "Export",NULL, NULL, NULL, 		0, NULL, NULL, ExportMenuBox },
	{ "Help",NULL, NULL, NULL, 		0, NULL, NULL, HelpMenuBox },
	{ "",NULL, NULL, NULL,			0, NULL, NULL, NULL }
};

MenuBox ContextMainMenuBox[] = {
	{ "File",NULL, NULL, NULL,		0, NULL, NULL, ContextFileMenuBox },
	{ "Edit",NULL, NULL, NULL, 		0, NULL, NULL, ContextEditMenuBox },
	{ "View",NULL, NULL, NULL, 		0, NULL, NULL, ContextViewMenuBox },
	{ "Search",NULL, NULL, NULL,  	0, NULL, NULL, ContextSearchMenuBox },
	{ "Export",NULL, NULL, NULL, 		0, NULL, NULL, ContextExportMenuBox },
	{ "Help",NULL, NULL, NULL, 		0, NULL, NULL, ContextHelpMenuBox },
	{ "",NULL, NULL, NULL,			0, NULL, NULL, NULL }
};

Menu::Menu(Controller *_controller) : Widget(_controller) {
	popup_menu = create_context_menu(ContextMainMenuBox);
	widget = create_menu(MainMenuBox);
	gtk_widget_show_all(widget);
}

Menu::~Menu() {
}

GtkWidget* Menu::create_menu(MenuBox *box) {
	int c = 0; GtkWidget *widget, *hbox, *label, *accel, *spacer;
	if (!box) box = MainMenuBox;
	
	if (box==MainMenuBox)
		widget = gtk_menu_bar_new();
	else
		widget = gtk_menu_new();
	gtk_widget_show(widget);
	
	for (c=0;box[c].name[0];c++) {
			hbox = gtk_hbox_new(FALSE,0);
		
			if (box[c].own_pix) {
				gtk_box_pack_start(GTK_BOX(hbox),gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_xpm_data(box[c].own_pix)),FALSE,FALSE,0);
			} else if (box[c].stock_id) {
				gtk_box_pack_start(GTK_BOX(hbox),gtk_image_new_from_stock(box[c].stock_id,TOOLBAR_ICON_SIZE), FALSE, FALSE, 0);
			}
			label = gtk_label_new(box[c].name);
			gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,3);
			if (box[c].accel) {
				spacer = gtk_label_new("");
				gtk_box_pack_start(GTK_BOX(hbox),spacer,TRUE,TRUE,8);
				accel = gtk_label_new(box[c].accel);
				gtk_box_pack_end(GTK_BOX(hbox),accel,FALSE,FALSE,0);
			}
			box[c].entry = gtk_menu_item_new();
			gtk_container_add(GTK_CONTAINER(box[c].entry),hbox);
			gtk_widget_show(box[c].entry);
	//		printf("Creating menu item %s\n",box[c].name);
		if (box[c].items != NULL) {
			box[c].menu = create_menu(box[c].items);
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(box[c].entry),box[c].menu);
		} else {
			g_object_set_data(G_OBJECT(box[c].entry),"command",(gpointer) box[c].command);
			g_signal_connect(G_OBJECT(box[c].entry),"activate",G_CALLBACK(Menu::activate_cb),this);
		}
		if (box==MainMenuBox)
			gtk_menu_bar_append(GTK_MENU_BAR(widget),box[c].entry);
		else
			gtk_menu_append(GTK_MENU(widget),box[c].entry);
	}
	return widget;
}

GtkWidget* Menu::create_context_menu(MenuBox *box) {
	int c = 0; GtkWidget *widget; // , *hbox, *label, *accel, *spacer;
	if (!box) box = MainMenuBox;
	
	if (box==MainMenuBox)
		widget = gtk_menu_bar_new();
	else
		widget = gtk_menu_new();
	gtk_widget_show(widget);
	
	for (c=0;box[c].name[0];c++) {
		/*
			hbox = gtk_hbox_new(FALSE,0);
		
			if (box[c].own_pix) {
				gtk_box_pack_start(GTK_BOX(hbox),gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_xpm_data(box[c].own_pix)),FALSE,FALSE,0);
			} else if (box[c].stock_id) {
				gtk_box_pack_start(GTK_BOX(hbox),gtk_image_new_from_stock(box[c].stock_id,TOOLBAR_ICON_SIZE), FALSE, FALSE, 0);
			}
			label = gtk_label_new(box[c].name);
			gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,3);
			if (box[c].accel) {
				spacer = gtk_label_new("");
				gtk_box_pack_start(GTK_BOX(hbox),spacer,TRUE,TRUE,8);
				accel = gtk_label_new(box[c].accel);
				gtk_box_pack_end(GTK_BOX(hbox),accel,FALSE,FALSE,0);
			}
			gtk_container_add(GTK_CONTAINER(box[c].entry),hbox);
		*/	
			box[c].entry = gtk_menu_item_new_with_label(box[c].name);
			gtk_widget_show(box[c].entry);
	//		printf("Creating menu item %s\n",box[c].name);
		if (box[c].items != NULL) {
			box[c].menu = create_context_menu(box[c].items);
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(box[c].entry),box[c].menu);
		} else {
			g_object_set_data(G_OBJECT(box[c].entry),"command",(gpointer) box[c].command);
			g_signal_connect(G_OBJECT(box[c].entry),"activate",G_CALLBACK(Menu::activate_cb),this);
		}
		if (box==MainMenuBox)
			gtk_menu_bar_append(GTK_MENU_BAR(widget),box[c].entry);
		else
			gtk_menu_append(GTK_MENU(widget),box[c].entry);
	}
	return widget;
}

void Menu::activate_cb(GtkMenuItem *item, gpointer data) {
	if (data) ((Menu*)data)->activate(item);
}

void Menu::activate(GtkMenuItem *item) {
	int command;
	
	command = (int) g_object_get_data(G_OBJECT(item),"command");
//	printf("Activation Command %i\n", command);
	if (controller) controller->executeCommand((Command) command);
}

void Menu::setActivations(Activation *activations, MenuBox *box) {
	int c;
	for (c=0;box[c].name[0];c++) {
		if (box[c].items)
			setActivations(activations,box[c].items);
		else
			gtk_widget_set_sensitive(box[c].entry,activations[(int) g_object_get_data(G_OBJECT(box[c].entry),"command")]);
	}
}

void Menu::setActivations(Activation *activations) {
	setActivations(activations, MainMenuBox);
	setActivations(activations, ContextMainMenuBox);
}

GtkWidget* Menu::getPopupMenu() { return popup_menu; }

#endif

