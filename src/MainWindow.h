#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include "Widget.h"
#include "TreeView.h"
#include "ListView.h"
#include "TimeView.h"
#include "Menu.h"
#include "Controller.h"
#include "Editor.h"
#include "KnowdeFile.h"
#include "Toolbar.h"
#include "Reader.h"
#include "Statusbar.h"
#include "Searcher.h"
#include "Abouter.h"
#include "ExportHTML.h"

class MainWindow : public Widget, public Controller {
	public:
		MainWindow(Controller *_controller);
		virtual ~MainWindow();
		void executeCommand(Command command);
		void updateActivations();
		void updateKnowdeDisplay(Knowde *knowde);
		void updateEditedKnowde(Knowde *knowde);
		void showPopupMenu(GtkWidget *widget);
		void centerWindow(GtkWidget *widget);
		void getWindowCoordinates(int *x, int *y, int *width, int *height);
		void executeSearch();
		
		int loadFile(char *filename);
		int closeFile();
		int saveFile(char *filename = NULL);
		void quit();
		KnowdeFile* getCurrentKnowdeFile();
		
	protected:
		
		gboolean 		changed;
		int				treeview;
		char 				search[512];
		char 				title[512];
		char				filename[512];
		Activation 		activations[N_COMMANDS];
		
		Menu*	 			menu;
		Toolbar*	 		toolbar;
		TreeView* 		tree_view;
		ListView* 		list_view;
		TimeView*		time_view;
		Reader* 			reader;
		Statusbar* 		statusbar;

		Editor* 			editor;
		Searcher* 		searcher;
		Abouter*			abouter;
		KnowdeFile* 	file;
//		GtkWidget* 		file_dialog;
		GtkWidget		*tree_scroller, *list_scroller, *time_scroller, *hpaned;
		Knowde* 			clipboard;
		Knowde*			last_found;

		int		checkSave();
		void 		updateTitle();
		void		clearClipboard();
		int		showQuestionDialog(char *msg);
		void		showErrorDialog(char *msg);
		int 		showFileSelectionDialog(char *buf, int bufsize);
		void		selectKnowde(Knowde *knowde);
		Knowde*	getSelectedKnowde();
		void		unselectAll();
		gboolean keyPressed(int state, int keyval);
		static 	gboolean destroy_cb(GtkWidget *widget, gpointer data);
		static	gboolean keypress_cb(GtkWidget *widget, GdkEventKey *event, gpointer data);
};

#endif

