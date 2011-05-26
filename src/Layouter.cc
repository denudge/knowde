#include "Layouter.h"

Layouter::Layouter(Controller *_controller) : Widget(_controller) {
	knowde = 0;
	has_font = 0;
	has_file_font = 0;
	font[0] = 0;
	file_font[0] = 0;
	has_text_color = 0;
	has_bg_color = 0;
	
	widget = gtk_vbox_new(FALSE,0);
	
	default_font_button = gtk_button_new_with_label("Default Font");
	font_button = gtk_button_new_with_label("Choose Font...");
	font_hbox = gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(font_hbox),default_font_button,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(font_hbox),font_button,FALSE,FALSE,5);
	font_vbox = gtk_vbox_new(0,0);
	font_label = gtk_label_new("-- default font --");
	gtk_box_pack_start(GTK_BOX(font_vbox),font_label,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(font_vbox),font_hbox,FALSE,FALSE,5);
	font_frame = gtk_frame_new("Text Font");
	gtk_container_add(GTK_CONTAINER(font_frame),font_vbox);
	
	system_font_button = gtk_button_new_with_label("System Default");
	file_font_button = gtk_button_new_with_label("Choose Font...");
	file_font_hbox = gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(file_font_hbox),system_font_button,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(file_font_hbox),file_font_button,FALSE,FALSE,5);
	file_font_vbox = gtk_vbox_new(0,0);
	file_font_label = gtk_label_new("-- system default font --");
	gtk_box_pack_start(GTK_BOX(file_font_vbox),file_font_label,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(file_font_vbox),file_font_hbox,FALSE,FALSE,5);
	file_font_frame = gtk_frame_new("This File's Default Text Font");
	gtk_container_add(GTK_CONTAINER(file_font_frame),file_font_vbox);
	
	default_text_color_button = gtk_button_new_with_label("Default Color");
	text_color_button = gtk_button_new_with_label("Choose Color...");
	text_color_widget = gtk_drawing_area_new();
	gtk_widget_set_size_request(text_color_widget,20,20);
	text_color_hbox = gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(text_color_hbox),default_text_color_button,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(text_color_hbox),text_color_button,FALSE,FALSE,5);
	text_color_vbox = gtk_vbox_new(0,0);	
	gtk_box_pack_start(GTK_BOX(text_color_vbox),text_color_widget,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(text_color_vbox),text_color_hbox,FALSE,FALSE,5);
	text_color_frame = gtk_frame_new("Text Color");
	gtk_container_add(GTK_CONTAINER(text_color_frame),text_color_vbox);
	
	default_bg_color_button = gtk_button_new_with_label("Default Color");
	bg_color_button = gtk_button_new_with_label("Choose Color...");
	bg_color_widget = gtk_drawing_area_new();
	gtk_widget_set_size_request(bg_color_widget,20,20);
	bg_color_hbox = gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(bg_color_hbox),default_bg_color_button,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(bg_color_hbox),bg_color_button,FALSE,FALSE,5);
	bg_color_vbox = gtk_vbox_new(0,0);	
	gtk_box_pack_start(GTK_BOX(bg_color_vbox),bg_color_widget,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(bg_color_vbox),bg_color_hbox,FALSE,FALSE,5);
	bg_color_frame = gtk_frame_new("Background Color");
	gtk_container_add(GTK_CONTAINER(bg_color_frame),bg_color_vbox);
	
	gtk_box_pack_start(GTK_BOX(widget),font_frame,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(widget),file_font_frame,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(widget),text_color_frame,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(widget),bg_color_frame,FALSE,FALSE,5);

	g_signal_connect(G_OBJECT(font_button),"clicked",G_CALLBACK(Layouter::font_cb),this);
	g_signal_connect(G_OBJECT(default_font_button),"clicked",G_CALLBACK(Layouter::default_font_cb),this);
	g_signal_connect(G_OBJECT(file_font_button),"clicked",G_CALLBACK(Layouter::file_font_cb),this);
	g_signal_connect(G_OBJECT(system_font_button),"clicked",G_CALLBACK(Layouter::system_font_cb),this);
	g_signal_connect(G_OBJECT(text_color_button),"clicked",G_CALLBACK(Layouter::text_color_cb),this);
	g_signal_connect(G_OBJECT(default_text_color_button),"clicked",G_CALLBACK(Layouter::default_text_color_cb),this);
	g_signal_connect(G_OBJECT(bg_color_button),"clicked",G_CALLBACK(Layouter::bg_color_cb),this);
	g_signal_connect(G_OBJECT(default_bg_color_button),"clicked",G_CALLBACK(Layouter::default_bg_color_cb),this);
}

Layouter::~Layouter() { }

void Layouter::applyValues() {
	if (!knowde) return;
	if (has_font) knowde->setTextFont(font);
	else knowde->disableTextFont();
	
	if ((controller) && (controller->getCurrentKnowdeFile())) {
		if (has_file_font)
			controller->getCurrentKnowdeFile()->setDefaultTextFont(file_font);
		else
			controller->getCurrentKnowdeFile()->disableDefaultTextFont();
	}
	if (has_text_color) knowde->setTextColor(&text_color);
		else knowde->disableTextColor();
	if (has_bg_color) knowde->setBackgroundColor(&bg_color);
		else knowde->disableBackgroundColor();
}

void Layouter::hide() {
	knowde = NULL;
	has_font = 0;
	has_file_font = 0;
	font[0] = 0;
	file_font[0] = 0;
	has_text_color = 0;
	has_bg_color = 0;
}

void Layouter::show(Knowde *_knowde) {
	knowde = _knowde;
	if (!knowde) return;
	
	if (knowde->hasTextFont()) {
		has_font = 1;
		strncpy(font,knowde->getTextFont(),511);
		font[511]=0;
	} else { has_font = 0; font[0] = 0; }
	
	if ((controller) && (controller->getCurrentKnowdeFile()) && (controller->getCurrentKnowdeFile()->hasDefaultTextFont())) {
		has_file_font = 1;
		strncpy(file_font,controller->getCurrentKnowdeFile()->getDefaultTextFont(),511);
		file_font[511]=0;
	} else { has_file_font = 0; file_font[0] = 0; }
	
	
	if (knowde->hasTextColor()) {
		has_text_color = 1;
		memcpy(&text_color,knowde->getTextColor(),sizeof(GdkColor));
	} else has_text_color = 0;
	if (knowde->hasBackgroundColor()) {
		has_bg_color = 1;
		memcpy(&bg_color, knowde->getBackgroundColor(), sizeof(GdkColor));
	} else has_bg_color = 0;
	update();
	gtk_widget_show(widget);
}

void Layouter::update() {
	PangoFontDescription *font_desc;
	
	gtk_widget_modify_font(font_label, NULL);
	gtk_widget_modify_font(file_font_label, NULL);
	
	if (has_font) {
		font_desc = pango_font_description_from_string(font);
		gtk_widget_modify_font(font_label,font_desc);
		pango_font_description_free(font_desc);
		gtk_label_set_text(GTK_LABEL(font_label),font);
	} else gtk_label_set_text(GTK_LABEL(font_label),"-- default font --");
	
	if (has_file_font) {
		font_desc = pango_font_description_from_string(file_font);
		gtk_widget_modify_font(file_font_label,font_desc);
		pango_font_description_free(font_desc);
		gtk_label_set_text(GTK_LABEL(file_font_label),file_font);
	} else gtk_label_set_text(GTK_LABEL(file_font_label),"-- system default font --");
	
	gtk_widget_modify_bg(bg_color_widget, GTK_STATE_NORMAL, NULL);
	if (has_bg_color) gtk_widget_modify_bg(bg_color_widget, GTK_STATE_NORMAL, &bg_color);
	gtk_widget_modify_bg(text_color_widget, GTK_STATE_NORMAL, NULL);
	if (has_text_color) gtk_widget_modify_bg(text_color_widget, GTK_STATE_NORMAL, &text_color);
}

void Layouter::chooseFont() {
	GtkWidget *font_dlg = gtk_font_selection_dialog_new(_("Choose Font..."));
	switch(gtk_dialog_run(GTK_DIALOG(font_dlg))) {
		case GTK_RESPONSE_OK:
			has_font = 1;
			strncpy(font,gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(font_dlg)),511);
			font[511] = 0;
			update();
			break;
		default: break;
	}
	gtk_widget_destroy(font_dlg);
}

void Layouter::defaultFont() {
	if (knowde) {
		has_font = 0;
		font[0] = 0;
		update();
	}
}

void Layouter::chooseFileFont() {
	GtkWidget *font_dlg = gtk_font_selection_dialog_new(_("Choose Font..."));
	switch(gtk_dialog_run(GTK_DIALOG(font_dlg))) {
		case GTK_RESPONSE_OK:
			has_file_font = 1;
			strncpy(file_font,gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(font_dlg)),511);
			file_font[511] = 0;
			update();
			break;
		default: break;
	}
	gtk_widget_destroy(font_dlg);
}

void Layouter::systemFont() {
	if (knowde) {
		has_file_font = 0;
		file_font[0] = 0;
		update();
	}
}

void Layouter::chooseTextColor() {
	GtkWidget *color_dlg = gtk_color_selection_dialog_new("Choose Color...");
	switch(gtk_dialog_run(GTK_DIALOG(color_dlg))) {
		case GTK_RESPONSE_OK:
			gtk_color_selection_get_current_color(
					GTK_COLOR_SELECTION(
						GTK_COLOR_SELECTION_DIALOG(color_dlg)->colorsel
					),
					&text_color
			);
			has_text_color = 1;
			update();
			break;
		default: break;	
	}
	gtk_widget_destroy(color_dlg);
}

void Layouter::defaultTextColor() {
	has_text_color = 0;
	update();
}

void Layouter::chooseBackgroundColor() {
	GtkWidget *color_dlg = gtk_color_selection_dialog_new("Choose Color...");
	switch(gtk_dialog_run(GTK_DIALOG(color_dlg))) {
		case GTK_RESPONSE_OK:
			gtk_color_selection_get_current_color(
					GTK_COLOR_SELECTION(
						GTK_COLOR_SELECTION_DIALOG(color_dlg)->colorsel
					),
					&bg_color
			);
			has_bg_color = 1;
			update();
			break;
		default: break;	
	}
	gtk_widget_destroy(color_dlg);
}

void Layouter::defaultBackgroundColor() {
	has_bg_color = 0;
	update();
}

void Layouter::font_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Layouter*) data)->chooseFont();
}

void Layouter::default_font_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Layouter*) data)->defaultFont();
}

void Layouter::file_font_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Layouter*) data)->chooseFileFont();
}

void Layouter::system_font_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Layouter*) data)->systemFont();
}

void Layouter::text_color_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Layouter*) data)->chooseTextColor();
}

void Layouter::default_text_color_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Layouter*) data)->defaultTextColor();
}

void Layouter::bg_color_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Layouter*) data)->chooseBackgroundColor();
}

void Layouter::default_bg_color_cb(GtkWidget *widget, gpointer data) {
	if (data) ((Layouter*) data)->defaultBackgroundColor();
}


