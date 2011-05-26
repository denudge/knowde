#ifndef __STATUSBAR_H__
#define __STATUSBAR_H__

#include "Widget.h"
#include "Knowde.h"

class Statusbar : public Widget {
	public:
		Statusbar(Controller *_controller);
		virtual ~Statusbar();
		void show(Knowde *knowde);

	protected:
		GtkWidget *size_label;
		void setText(char *text);
		static void humanizeNumber(unsigned long number, char *dest);
};

#endif

