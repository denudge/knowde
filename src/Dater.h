#ifndef __DATER_H__
#define __DATER_H__

#include "config.h"
#include "Widget.h"

class Dater : public Widget {
	public:
		Dater(Controller *_controller);
		virtual ~Dater();

		void show(Knowde *_knowde);
		void hide();
		void applyValues();

	protected:
		Knowde *knowde;
		DatePoint *start_date, *end_date;
		GtkWidget *date_flag, *durative_flag;
		GtkWidget *start_year, *start_month, *start_day;
		GtkWidget *end_year, *end_month, *end_day;
		GtkWidget *vbox, *start_box, *end_box;
		GtkWidget *start_frame, *end_frame;
		GtkWidget *year_label, *month_label, *day_label;

		void update();
		static void toggled_cb(GtkToggleButton *button, gpointer data);
};

#endif

