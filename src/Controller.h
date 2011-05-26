#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "config.h"
#include <gtk/gtk.h>
#include "Knowde.h"
#include "KnowdeFile.h"

class Controller {
	public:
		virtual void executeCommand(Command command) =0;
		virtual void updateActivations() =0;
		virtual void selectKnowde(Knowde *knowde) =0;
		virtual void updateKnowdeDisplay(Knowde *knowde) =0;
		virtual void updateEditedKnowde(Knowde *knowde) =0;
		virtual void showPopupMenu(GtkWidget *widget) =0;
		virtual void getWindowCoordinates(int *x, int *y, int *width, int *height) =0;
		virtual void centerWindow(GtkWidget *widget) =0;
		virtual void executeSearch() =0;

		virtual KnowdeFile* getCurrentKnowdeFile() =0;
//		virtual void executeResponse() =0;
};

#endif

