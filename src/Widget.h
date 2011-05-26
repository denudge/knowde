#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <gtk/gtk.h>
#include "config.h"
#include "Controller.h"

class Widget {
	public:
		Widget(Controller *_controller) { controller = _controller; }
		virtual ~Widget() { controller = NULL; }
		GtkWidget* getWidget() { return widget; }

	protected:
		GtkWidget *widget;
		Controller *controller;
};

#endif

