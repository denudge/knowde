#ifndef __EDITOR_CC__
#define __EDITOR_CC__

#include "Editor.h"
#include <glib.h>

#define DEFAULT_EDITOR_WIDTH 400
#define DEFAULT_EDITOR_HEIGHT 400

Editor::Editor(Controller *_controller) : Widget(_controller),
	dater(_controller), layouter(_controller) {

	knowde = NULL;
	widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(widget,DEFAULT_EDITOR_WIDTH,DEFAULT_EDITOR_HEIGHT);
	gtk_window_set_modal(GTK_WINDOW(widget), TRUE);

	title_label = gtk_label_new(_("Title: "));
	descr_label = gtk_label_new(_("Description: "));
	keywords_label = gtk_label_new(_("Keywords: "));
	gtk_label_set_justify(GTK_LABEL(title_label),GTK_JUSTIFY_LEFT);
	gtk_label_set_justify(GTK_LABEL(descr_label),GTK_JUSTIFY_LEFT);
	gtk_label_set_justify(GTK_LABEL(keywords_label),GTK_JUSTIFY_LEFT);

	label_box = gtk_vbox_new(TRUE,0);
	gtk_box_pack_start(GTK_BOX(label_box),title_label,TRUE,FALSE,5);
	gtk_box_pack_end(GTK_BOX(label_box),descr_label,TRUE,FALSE,5);
	gtk_box_pack_end(GTK_BOX(label_box),keywords_label,TRUE,FALSE,5);

	title = gtk_entry_new();
	descr = gtk_entry_new();
	keywords = gtk_entry_new();
	
	entry_box = gtk_vbox_new(TRUE,0);
	gtk_box_pack_start(GTK_BOX(entry_box),title,TRUE,TRUE,5);
	gtk_box_pack_end(GTK_BOX(entry_box),descr,TRUE,TRUE,5);
	gtk_box_pack_end(GTK_BOX(entry_box),keywords,TRUE,TRUE,5);

	header_box = gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(header_box),label_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(header_box),entry_box,TRUE,TRUE,0);
	
	header_frame = gtk_frame_new(_("Meta Information"));
	gtk_container_add(GTK_CONTAINER(header_frame),header_box);

	layout_flag = gtk_check_button_new_with_label(_("Use Layout"));
	text = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text), GTK_WRAP_WORD);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
/*	
	if ((GTK_TEXT_VIEW(text)->vadjustment) != NULL) {
		printf("Adjustment was there!\n");
	} else printf("Adjustment was NULL!\n");
*/
//	hadj = (GtkAdjustment*) gtk_adjustment_new(0.0,0.0,1.0,0.01,0.1,0.1);
		// (GTK_TEXT_VIEW(text))->hadjustment = hadj;
//	vadj = (GtkAdjustment*) gtk_adjustment_new(0.0,0.0,1.0,0.01,0.1,0.1);
		// (GTK_TEXT_VIEW(text))->vadjustment = vadj;

/*
	text_scroller = gtk_scrolled_window_new(hadj, vadj);
	gtk_scrolled_window_add_with_viewport((GtkScrolledWindow*) text_scroller, text);
	gtk_scrolled_window_set_policy((GtkScrolledWindow*) text_scroller, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
*/	
	text_vbox = gtk_vbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(text_vbox), layout_flag, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(text_vbox), text, TRUE, TRUE, 0);
	text_frame = gtk_frame_new(_("Text: "));
	gtk_container_add(GTK_CONTAINER(text_frame),text_vbox);

	cancel_button = gtk_button_new_with_label(_("Cancel"));
	apply_button = gtk_button_new_with_label(_("Apply"));
	ok_button = gtk_button_new_with_label(_("OK"));

	button_box = gtk_hbox_new(0,1);
	gtk_box_pack_end(GTK_BOX(button_box),cancel_button,FALSE,FALSE,5);
	gtk_box_pack_end(GTK_BOX(button_box),apply_button,FALSE,FALSE,5);
	gtk_box_pack_end(GTK_BOX(button_box),ok_button,FALSE,FALSE,5);
	
	editor_box = gtk_vbox_new(0,0);
	editor_label = gtk_label_new(_("General"));
	gtk_box_pack_start(GTK_BOX(editor_box),header_frame,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(editor_box),text_frame,TRUE,TRUE,5);
	gtk_widget_show_all(editor_box);

	dater_label = gtk_label_new("Date");
	layouter_label = gtk_label_new("Layout");
	
	vbox = gtk_vbox_new(0,0);
	notebook = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),editor_box,editor_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),dater.getWidget(),dater_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),layouter.getWidget(),layouter_label);
	gtk_box_pack_start(GTK_BOX(vbox),notebook,TRUE,TRUE,0);
	gtk_box_pack_end(GTK_BOX(vbox),button_box,FALSE,FALSE,5);
	gtk_widget_show_all(vbox);
	gtk_container_add(GTK_CONTAINER(widget),vbox);
	
	g_signal_connect(G_OBJECT(widget),"delete-event",G_CALLBACK(Editor::delete_event_cb),this);
	g_signal_connect(G_OBJECT(ok_button),"clicked",G_CALLBACK(Editor::ok_cb),this);
	g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(Editor::cancel_cb),this);
	g_signal_connect(G_OBJECT(apply_button),"clicked",G_CALLBACK(Editor::apply_cb),this);
	g_signal_connect(G_OBJECT(layout_flag),"toggled",G_CALLBACK(Editor::layout_cb),this);

	// g_signal_connect(G_OBJECT(buffer),"insert-text",G_CALLBACK(Editor::insert_text_cb),this);
}

Editor::~Editor() {
}

void Editor::show(Knowde *_knowde) {
	if (!_knowde) return;
	knowde = _knowde;

	gtk_entry_set_text(GTK_ENTRY(title),knowde->getTitle());
	gtk_entry_set_text(GTK_ENTRY(descr),knowde->getDescr());
	gtk_entry_set_text(GTK_ENTRY(keywords),knowde->getKeywords());
	gtk_text_buffer_set_text(buffer,knowde->getText(), knowde->getTextSize());
	
	update();
	dater.show(_knowde);
	layouter.show(_knowde);
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook),0);
	if (controller) 
		controller->centerWindow(widget);
	gtk_widget_show(widget);
}

void Editor::update() {
	if (!knowde) return;
	PangoFontDescription *font_desc;
	
	// changing font
	gtk_widget_modify_font(text, NULL);
	gtk_widget_modify_base(text, GTK_STATE_NORMAL, NULL);
	gtk_widget_modify_text(text, GTK_STATE_NORMAL, NULL);
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(layout_flag))) {
		GtkTextIter start, end;
		gtk_text_buffer_get_start_iter(buffer,&start);
		gtk_text_buffer_get_end_iter(buffer,&end);
		if (knowde->hasTextFont()) {
			font_desc = pango_font_description_from_string (knowde->getTextFont());
			gtk_widget_modify_font(text, font_desc);
			pango_font_description_free(font_desc);
		} else if ((controller) && (controller->getCurrentKnowdeFile()) && (controller->getCurrentKnowdeFile()->hasDefaultTextFont())) {
			font_desc = pango_font_description_from_string(controller->getCurrentKnowdeFile()->getDefaultTextFont());
			gtk_widget_modify_font(text, font_desc);
			pango_font_description_free(font_desc);
		}

		// changing background and text color
		if (knowde->hasBackgroundColor())
			gtk_widget_modify_base(text, GTK_STATE_NORMAL, knowde->getBackgroundColor());
		if (knowde->hasTextColor())
			gtk_widget_modify_text(text, GTK_STATE_NORMAL, knowde->getTextColor());
	}
}

void Editor::hide() {
	gtk_widget_hide(widget);
	knowde = NULL;
}

void Editor::applyValues() {
	GtkTextIter start, end;
	if (!knowde) { printf(_("Knowde was NULL\n")); return; }
	knowde->setTitle((char*) gtk_entry_get_text(GTK_ENTRY(title)));
	knowde->setDescr((char*) gtk_entry_get_text(GTK_ENTRY(descr)));
	knowde->setKeywords((char*) gtk_entry_get_text(GTK_ENTRY(keywords)));
	gtk_text_buffer_get_start_iter(buffer,&start);
	gtk_text_buffer_get_end_iter(buffer,&end);
	knowde->setText((char*) gtk_text_buffer_get_text(buffer,&start,&end, FALSE));
	
	dater.applyValues();
	layouter.applyValues();
	update();

	if (controller)
		controller->updateEditedKnowde(knowde);
}

gboolean Editor::delete_event_cb(GtkWidget *widget, GdkEvent *event, gpointer data) {
	if (data) ((Editor*) data)->hide();
	return TRUE;
}

gboolean Editor::ok_cb(GtkWidget *widget, gpointer data) {
	if (data) {
		((Editor*) data)->applyValues();
		((Editor*) data)->hide();
	}
	return TRUE;
}

gboolean Editor::apply_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Editor*) data)->applyValues();
	return TRUE;
}

gboolean Editor::cancel_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Editor*) data)->hide();
	return TRUE;
}

void Editor::insert_text(GtkTextIter *iter) {
/*
	GdkRectangle rect; int width; float point;
	gtk_text_view_get_iter_location(GTK_TEXT_VIEW(text),iter, &rect);
//	gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(text),iter,0.5,FALSE,0.0,0.0);
	width 		= text->allocation.width;
	point = (float) rect.x / (float) width;
	printf("Point: %f\n",point);
	printf("Value: %f\n",gtk_adjustment_get_value(hadj));
	printf("New Value: %f\n",hadj->lower + (point * (hadj->upper-hadj->lower)));
	gtk_adjustment_set_value(hadj,hadj->lower + (point * (hadj->upper-hadj->lower)));
*/
}


gboolean Editor::insert_text_cb(GtkTextBuffer *buffer, GtkTextIter *pos, const gchar *text, gint length, gpointer data) {
	if (data) ((Editor*) data)->insert_text(pos);
	return FALSE;
}

void Editor::layout_cb(GtkToggleButton *button, gpointer data) {
	if (data) ((Editor*) data)->update();
}	

#endif
