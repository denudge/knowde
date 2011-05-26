#include "Dater.h"

Dater::Dater(Controller *_controller) : Widget(_controller) {

	date_flag = gtk_check_button_new_with_label(_("Enable Date"));
	durative_flag = gtk_check_button_new_with_label(_("Durative Date"));
	
	start_year = gtk_spin_button_new_with_range(-32000,+32000,1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(start_year), 2004);
	gtk_spin_button_set_increments(GTK_SPIN_BUTTON(start_year),1,100);
	
	start_month = gtk_spin_button_new_with_range(0,12,1);
	start_day = gtk_spin_button_new_with_range(0,31,1);	

	year_label = gtk_label_new(_("Year"));
	month_label = gtk_label_new(_("Month"));
	day_label = gtk_label_new(_("Day"));

	// the table of year/month/day values
	start_box = gtk_table_new(2,3,FALSE);
	gtk_table_attach(GTK_TABLE(start_box),year_label,0,1,0,1,GTK_FILL,GTK_FILL,2,2);
	gtk_table_attach(GTK_TABLE(start_box),month_label,1,2,0,1,GTK_FILL,GTK_FILL,2,2);
	gtk_table_attach(GTK_TABLE(start_box),day_label,2,3,0,1,GTK_FILL,GTK_FILL,2,2);
	gtk_table_attach(GTK_TABLE(start_box),start_year,0,1,1,2,GTK_FILL,GTK_FILL,2,2);
	gtk_table_attach(GTK_TABLE(start_box),start_month,1,2,1,2,GTK_FILL,GTK_FILL,2,2);
	gtk_table_attach(GTK_TABLE(start_box),start_day,2,3,1,2,GTK_FILL,GTK_FILL,2,2);
	
	start_frame = gtk_frame_new(_("Start Date"));
	gtk_container_add(GTK_CONTAINER(start_frame),start_box);

	// 2nd part: the end
	end_year = gtk_spin_button_new_with_range(-32000,+32000,1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(end_year), 2004);
	gtk_spin_button_set_increments(GTK_SPIN_BUTTON(end_year),1,100);
	
	end_month = gtk_spin_button_new_with_range(0,12,1);
	end_day = gtk_spin_button_new_with_range(0,31,1);	

	end_box = gtk_table_new(1,3,FALSE);
	gtk_table_attach(GTK_TABLE(end_box),end_year,0,1,0,1,GTK_FILL,GTK_FILL,2,2);
	gtk_table_attach(GTK_TABLE(end_box),end_month,1,2,0,1,GTK_FILL,GTK_FILL,2,2);
	gtk_table_attach(GTK_TABLE(end_box),end_day,2,3,0,1,GTK_FILL,GTK_FILL,2,2);
	
	end_frame = gtk_frame_new(_("End Date"));
	gtk_container_add(GTK_CONTAINER(end_frame),end_box);

	vbox = gtk_vbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),date_flag,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),start_frame,TRUE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),durative_flag,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),end_frame,TRUE,FALSE,5);

	gtk_widget_show_all(vbox);
	widget = gtk_vbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(widget),vbox,FALSE,FALSE,0);

	g_signal_connect(G_OBJECT(date_flag),"toggled",G_CALLBACK(Dater::toggled_cb),this);
	g_signal_connect(G_OBJECT(durative_flag),"toggled",G_CALLBACK(Dater::toggled_cb),this);
}

Dater::~Dater() { }

void Dater::update() {
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(date_flag))) {
		gtk_widget_set_sensitive(start_frame, TRUE);
		gtk_widget_set_sensitive(durative_flag, TRUE);
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(durative_flag))) {
			gtk_widget_set_sensitive(end_frame, TRUE);
		} else {
			gtk_widget_set_sensitive(end_frame, FALSE);
		}
	} else {
		gtk_widget_set_sensitive(start_frame, FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(durative_flag), FALSE);
		gtk_widget_set_sensitive(durative_flag, FALSE);	
		gtk_widget_set_sensitive(end_frame, FALSE);
	}
}

void Dater::applyValues() {
	if (!knowde) return;
	knowde->disableDate();
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(date_flag))) {
		knowde->enableDate();
		knowde->getStartDate()->setYear(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(start_year)));
		knowde->getStartDate()->setMonth((unsigned char) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(start_month)));
		knowde->getStartDate()->setDay((unsigned char) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(start_day)));
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(durative_flag))) {
			knowde->setDateDurative(TRUE);
			knowde->getEndDate()->setYear(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(end_year)));
			knowde->getEndDate()->setMonth((unsigned char) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(end_month)));
			knowde->getEndDate()->setDay((unsigned char) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(end_day)));
		}
	}
	knowde->normalizeDate();
}

void Dater::show(Knowde *_knowde) {
	knowde = _knowde;
	
	if (!knowde) return;

	// clear data
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(start_year), 2004);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(start_month), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(start_day), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(end_year), 2004);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(end_month), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(end_day), 0);

	// load values from the actual knowde;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(date_flag), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(durative_flag), FALSE);
	
	if (knowde->getStartDate()) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(date_flag),TRUE);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(start_year),knowde->getStartDate()->getYear());
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(start_month),knowde->getStartDate()->getMonth());
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(start_day),knowde->getStartDate()->getDay());
		if (knowde->getEndDate()) {
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(durative_flag),TRUE);
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(end_year),knowde->getEndDate()->getYear());
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(end_month),knowde->getEndDate()->getMonth());
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(end_day),knowde->getEndDate()->getDay());
		}
	}
	update();
	gtk_widget_show(widget);
}

void Dater::toggled_cb(GtkToggleButton *button, gpointer data) {
	if (data) {
		((Dater*) data)->update();
	}
}

