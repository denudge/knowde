#ifndef __LAYOUTER_H__
#define __LAYOUTER_H__

#include "config.h"
#include "Widget.h"

class Layouter : public Widget {
	public:
		Layouter(Controller *_controller);
		virtual ~Layouter();

		void show(Knowde *_knowde);
		void hide();
		void applyValues();
		void chooseFont();
		void defaultFont();
		void chooseFileFont();
		void systemFont();
		void chooseTextColor();
		void defaultTextColor();
		void chooseBackgroundColor();
		void defaultBackgroundColor();

	protected:
		Knowde 		*knowde;
		int 			has_font, has_file_font;
		int 			has_text_color;
		int 			has_bg_color;
		char 			font[512], file_font[512];
		GdkColor 	text_color;
		GdkColor 	bg_color;

		GtkWidget *font_button, *default_font_button;
		GtkWidget *font_label, *font_frame, *font_hbox, *font_vbox;

		GtkWidget *file_font_button, *system_font_button;
		GtkWidget *file_font_label, *file_font_frame, *file_font_hbox, *file_font_vbox;

		GtkWidget *text_color_button, *default_text_color_button, *text_color_vbox;
		GtkWidget *text_color_widget, *text_color_hbox, *text_color_frame;
		
		GtkWidget *bg_color_button, *default_bg_color_button, *bg_color_vbox;
		GtkWidget *bg_color_widget, *bg_color_hbox, *bg_color_frame;
		
		GtkWidget *test_label, *test_label_frame;
		
		void update();
		static void font_cb(GtkWidget *widget, gpointer data);
		static void default_font_cb(GtkWidget *widget, gpointer data);
		static void file_font_cb(GtkWidget *widget, gpointer data);
		static void system_font_cb(GtkWidget *widget, gpointer data);
		static void text_color_cb(GtkWidget *widget, gpointer data);
		static void default_text_color_cb(GtkWidget *widget, gpointer data);
		static void bg_color_cb(GtkWidget *widget, gpointer data);
		static void default_bg_color_cb(GtkWidget *widget, gpointer data);
};

#endif

