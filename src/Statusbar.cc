#ifndef __STATUSBAR_CC__
#define __STATUSBAR_CC__

#include "Statusbar.h"

Statusbar::Statusbar(Controller *_controller) : Widget(_controller) {
	size_label = gtk_label_new("");
	
	widget = gtk_hbox_new(FALSE,10);
	
	gtk_box_pack_end(GTK_BOX(widget),size_label,FALSE,FALSE,0);
	gtk_widget_show_all(widget);
}

Statusbar::~Statusbar() {
}

void Statusbar::humanizeNumber(unsigned long number, char *dest) {
	float h = number; int stage=0;
	
	while (h >= 1024.0) { 
		h /= 1024.0;
		stage++;
	}

	switch (stage) {
		case 0: sprintf(dest,"%lu Bytes", number); break;
		case 1: sprintf(dest,"%.1fK", h); break;
		case 2: sprintf(dest,"%.1fM", h); break;
		case 3: sprintf(dest,"%.1fG", h); break;
		case 4: sprintf(dest,"%.1fT", h); break;
		default: sprintf(dest,"%lu Bytes", number); break;
	}

}

void Statusbar::show(Knowde *knowde) {
	char size[16], size_rec[16], msg[96];

	humanizeNumber(knowde->getSize(),size);
	humanizeNumber(knowde->getSizeRecoursive(),size_rec);
	
	sprintf(msg,"%s: %s. %s: %s",_("Knowde Size"),size,_("Sum"), size_rec);
	setText(msg);
}

void Statusbar::setText(char *text) {
	gtk_label_set_text(GTK_LABEL(size_label),(gchar*) text);
}

#endif

