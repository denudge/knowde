#ifndef __ABOUTER_CC__
#define __ABOUTER_CC__

#include "Abouter.h"

Abouter::Abouter(Controller *_controller) : Widget(_controller) {
	char buf[512];
	widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	vbox = gtk_vbox_new(FALSE,0);
	ok_button = gtk_button_new_with_label(_("OK"));
	copyright_label = gtk_label_new(APP_COPYRIGHT);
	email_label = gtk_label_new(APP_EMAIL);
	program_label = gtk_label_new(APP_NAME);
	version_label = gtk_label_new(APP_VERSION);
	gnu_label = gtk_label_new(APP_GNU);
	sprintf(buf,"Last Edited: %s",APP_DATE);
	date_label = gtk_label_new(buf);
	
	gtk_box_pack_start(GTK_BOX(vbox),program_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),version_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),date_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),gnu_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),copyright_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),email_label,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(vbox),ok_button,FALSE,FALSE,0);

	gtk_widget_show_all(vbox);
	gtk_container_add(GTK_CONTAINER(widget),vbox);
	
	g_signal_connect(G_OBJECT(widget),"delete-event",G_CALLBACK(Abouter::delete_event_cb),this);
	g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(Abouter::ok_cb),this);
}

Abouter::~Abouter() { }

void Abouter::show() {
	if (controller)
		controller->centerWindow(widget);
	gtk_widget_show(widget);
}

void Abouter::hide() {
	gtk_widget_hide(widget);
}

gboolean Abouter::ok_cb(GtkWidget *widget, gpointer data) {
	if (data)
		((Abouter*) data)->hide();
	return TRUE;
}

gboolean Abouter::delete_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data) {
	if (data)
		((Abouter*) data)->hide();
	return TRUE;
}

#endif

