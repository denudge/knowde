#ifndef __SEARCHER_CC__
#define __SEARCHER_CC__

#include "config.h"
#include "Searcher.h"
#include <glib.h>

#define DEFAULT_SEARCHER_WIDTH 300
#define DEFAULT_SEARCHER_HEIGHT 180

Searcher::Searcher(Controller* _controller) : Widget(_controller) {
	widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(widget,DEFAULT_SEARCHER_WIDTH,DEFAULT_SEARCHER_HEIGHT);
	gtk_window_set_modal(GTK_WINDOW(widget), TRUE);

	vbox = gtk_vbox_new(0,0);

	entry_label = gtk_label_new(_("Search Expression"));
	entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(vbox),entry_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),entry,FALSE,FALSE,0);

	option_title = gtk_check_button_new_with_label(_("Title"));
	option_descr = gtk_check_button_new_with_label(_("Description"));
	option_text = gtk_check_button_new_with_label(_("Text"));
	option_keywords = gtk_check_button_new_with_label(_("Keywords"));
	option_case = gtk_check_button_new_with_label(_("Case Sensitive"));

	// defaults
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(option_title),TRUE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(option_descr),TRUE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(option_text),TRUE);

	option_box = gtk_table_new(4,2,TRUE);
	gtk_table_attach(GTK_TABLE(option_box),option_title,0,1,0,1,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(GTK_TABLE(option_box),option_descr,0,1,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(GTK_TABLE(option_box),option_text,0,1,2,3,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(GTK_TABLE(option_box),option_keywords,0,1,3,4,GTK_FILL,GTK_FILL,0,0);
	gtk_table_attach(GTK_TABLE(option_box),option_case,1,2,0,1,GTK_FILL,GTK_FILL,0,0);

	option_frame = gtk_frame_new("Options");
	gtk_container_add(GTK_CONTAINER(option_frame),option_box);
	gtk_box_pack_start(GTK_BOX(vbox),option_frame,TRUE,TRUE,5);

	ok_button = gtk_button_new_with_label(_("Search"));
	cancel_button = gtk_button_new_with_label(_("Cancel"));
	button_box = gtk_hbox_new(0,0);
	gtk_box_pack_end(GTK_BOX(button_box), cancel_button, FALSE, FALSE, 5);
	gtk_box_pack_end(GTK_BOX(button_box), ok_button, FALSE, FALSE, 5);
	gtk_box_pack_end(GTK_BOX(vbox), button_box, FALSE, FALSE, 5);
	
	g_signal_connect(G_OBJECT(widget),"delete-event",G_CALLBACK(Searcher::delete_event_cb),this);
	g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(Searcher::ok_cb),this);
	g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(Searcher::cancel_cb),this);
	
	gtk_widget_show_all(vbox);
	gtk_container_add(GTK_CONTAINER(widget),vbox);
	
}

Searcher::~Searcher() {
	// g_free(widget);
	// g_free(vbox);
}

void Searcher::show() {
	if (controller)
		controller->centerWindow(widget);
	gtk_widget_show(widget);
}

void Searcher::hide() {
	gtk_widget_hide(widget);
}

void Searcher::executeSearch() {
//	printf("Searcher::executeSearch()\n");
	strcpy(expression,gtk_entry_get_text(GTK_ENTRY(entry)));
	if (controller)
		controller->executeSearch();
}

char* Searcher::getSearchExpression() {
	return expression;
}

int Searcher::isTitle() {
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(option_title));
}

int Searcher::isDescription() {
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(option_descr));
}

int Searcher::isText() {
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(option_text));
}

int Searcher::isKeywords() {
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(option_keywords));
}

int Searcher::isCaseSensitive() {
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(option_case));
}

gboolean Searcher::ok_cb(GtkWidget *_widget, gpointer data) {
	if (data) {
		((Searcher*) data)->executeSearch();
		((Searcher*) data)->hide();
	}
	return TRUE;
}

gboolean Searcher::cancel_cb(GtkWidget *_widget, gpointer data) {
	if (!data) return TRUE;
	((Searcher*) data)->hide();
	return TRUE;
}

gboolean Searcher::delete_event_cb(GtkWidget *_widget, GdkEvent *event, gpointer data) {
	if (data) ((Searcher*) data)->hide();
	return TRUE;
}

#endif

