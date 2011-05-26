#ifndef __MAIN_WINDOW_CC__
#define __MAIN_WINDOW_CC__

#include <stdio.h>
#include <stdlib.h>
#include "MainWindow.h"
#include "ExportHTML.h"
#include "config.h"

MainWindow::MainWindow(Controller *_controller) : Widget(_controller) {
	
	GtkWidget *vbox; GtkWidget *hbox;

	changed 			= FALSE;
	treeview 		= 0;
	clipboard 		= NULL;
	last_found 		= NULL;

	widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_add_events(widget, GDK_KEY_PRESS_MASK);

	controller 	= this;
	editor 		= new Editor(this);
	menu			= new Menu(this);
	toolbar		= new Toolbar(this);
	reader		= new Reader(this);
	tree_view	= new TreeView(this);
	list_view	= new ListView(this);
	time_view 	= new TimeView(this);
	statusbar	= new Statusbar(this);

	searcher		= new Searcher(this);
	abouter		= new Abouter(this);

	tree_scroller = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_add_with_viewport((GtkScrolledWindow*) tree_scroller, tree_view->getWidget());
	gtk_scrolled_window_set_policy((GtkScrolledWindow*) tree_scroller, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_usize(tree_scroller,175,100);

	list_scroller = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_add_with_viewport((GtkScrolledWindow*) list_scroller, list_view->getWidget());
	gtk_scrolled_window_set_policy((GtkScrolledWindow*) list_scroller, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_usize(list_scroller,175,100);

	time_scroller = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_add_with_viewport((GtkScrolledWindow*) time_scroller, time_view->getWidget());
	gtk_scrolled_window_set_policy((GtkScrolledWindow*) time_scroller, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_usize(time_scroller,175,100);

	hpaned = gtk_hpaned_new();
	hbox = gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(hbox),tree_scroller, TRUE, TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),list_scroller, TRUE, TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),time_scroller, TRUE, TRUE,0);
	
	gtk_paned_add1(GTK_PANED(hpaned),hbox);
	gtk_paned_add2(GTK_PANED(hpaned),reader->getWidget());

	vbox = gtk_vbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(vbox),menu->getWidget(),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),toolbar->getWidget(),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),hpaned,TRUE,TRUE,0);
	gtk_box_pack_end(GTK_BOX(vbox),statusbar->getWidget(),FALSE,FALSE,0);

	g_signal_connect(G_OBJECT(widget),"destroy",G_CALLBACK(MainWindow::destroy_cb),this);
	g_signal_connect(G_OBJECT(widget),"key-press-event",G_CALLBACK(MainWindow::keypress_cb),this);
	
	gtk_widget_show_all(vbox);
	gtk_widget_hide(list_scroller);
	gtk_widget_hide(time_scroller);
	gtk_container_add(GTK_CONTAINER(widget),vbox);
	gtk_window_set_default_size(GTK_WINDOW(widget),640,440);
	
	reader->setKeywordTree(list_view->getTreeModel());
	updateActivations();
	gtk_widget_show(widget);
}

MainWindow::~MainWindow() {
	if (file) delete file;
}

int MainWindow::showQuestionDialog(char *msg) {
	GtkWidget *dlg; int response;
	dlg = gtk_message_dialog_new(GTK_WINDOW(widget), GTK_DIALOG_DESTROY_WITH_PARENT,
			                                    GTK_MESSAGE_QUESTION, 
															GTK_BUTTONS_NONE,
															msg);
	gtk_dialog_add_buttons(GTK_DIALOG(dlg), GTK_STOCK_YES, GTK_RESPONSE_YES,
															GTK_STOCK_NO, GTK_RESPONSE_NO,
															GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
	response = gtk_dialog_run(GTK_DIALOG(dlg));
	gtk_widget_destroy(dlg);
	return response;
}

void MainWindow::showErrorDialog(char *msg) {
	GtkWidget *dlg = gtk_message_dialog_new(GTK_WINDOW(widget), GTK_DIALOG_DESTROY_WITH_PARENT,
			                                    GTK_MESSAGE_ERROR, 
															GTK_BUTTONS_CLOSE,
															msg);
	gtk_dialog_run(GTK_DIALOG(dlg));
	gtk_widget_destroy(dlg);
}

int MainWindow::showFileSelectionDialog(char* buf, int bufsize) {
	GtkWidget *dlg; int response;
	dlg = gtk_file_selection_new(_("Choose a file"));
	response = gtk_dialog_run(GTK_DIALOG(dlg));
	switch (response) {
		case GTK_RESPONSE_OK: 
			strncpy(buf, gtk_file_selection_get_filename(GTK_FILE_SELECTION(dlg)), bufsize-1);
			gtk_widget_destroy(dlg);
			return TRUE;
		default:
			gtk_widget_destroy(dlg);
			return FALSE;
	}
}

int MainWindow::checkSave() {
	if (!file) return TRUE;
	if (!changed) return TRUE;
	
	int response = showQuestionDialog(_("Do you want to save your modified file?"));
	switch (response) {
		case GTK_RESPONSE_CANCEL: return FALSE;
		case GTK_RESPONSE_NO: return TRUE;
		case GTK_RESPONSE_YES: return saveFile();
		default: return FALSE;
	}
}

int MainWindow::saveFile(char *newFilename) {
	char buf[512];
	
	if (!file) return TRUE;
	
	// saveAs
	if (newFilename) {
		file->saveAs(newFilename);
		changed = FALSE;
		updateTitle();
		updateActivations();
		return TRUE;
	}

	// if file has name
	if (file->getFilename()[0]) {
		file->save();
		changed = FALSE;
		updateTitle();
		updateActivations();
		return TRUE;
	}
	
	// choose a new filename
	if (!showFileSelectionDialog(buf,512)) return FALSE;
	file->saveAs(buf);
	changed = FALSE;
	updateTitle();
	updateActivations();
	return TRUE;
}

int MainWindow::loadFile(char *filename) {
	char buf[512];
	
	if (!closeFile()) return FALSE;

	if (!filename) {
		if (!showFileSelectionDialog(buf,512)) return FALSE;
		file = new KnowdeFile(buf);
	} else file = new KnowdeFile(filename);
	if (!file) {
		showErrorDialog(_("Could not open file!\n"));
		return FALSE;
	}

	tree_view->loadFile(file);
	list_view->loadFile(file);
	time_view->loadFile(file);
	changed = FALSE;
	updateTitle();
	updateActivations();
	return TRUE;
}

int MainWindow::closeFile() {
	if (!file) {
		changed = FALSE;
		updateTitle();
		updateActivations();
		return TRUE;
	}
	if (!checkSave()) return FALSE;
	delete file; file = NULL;
	tree_view->clear();
	list_view->clear();
	time_view->clear();
	reader->clear();
	changed = FALSE;
	updateTitle();
	updateActivations();
	return TRUE;
}
		
KnowdeFile* MainWindow::getCurrentKnowdeFile() {
	return file;
}

void MainWindow::updateTitle() {
	if (!file) {
		gtk_window_set_title(GTK_WINDOW(widget),"Knowde");
		return;
	}
	sprintf(title,"Knowde - %s%s",file->getFilename(),(changed ? " (modified)":""));
	gtk_window_set_title(GTK_WINDOW(widget),title);
}

gboolean MainWindow::destroy_cb(GtkWidget *widget, gpointer data) {
	if (data) {
		((MainWindow*) data)->quit();
		return TRUE;
	}
	return FALSE;
};

gboolean MainWindow::keyPressed(int state, int keyval) {
	if (state==0) switch (keyval) {
		case 65535:
			executeCommand(CMD_EDIT_CUT);
			return TRUE;
		default:
			return FALSE;
	}

	// Ctrl-* key commands
	if (state==4) switch (keyval) {
		case 'c': executeCommand(CMD_EDIT_COPY); return TRUE;
		case 'e': executeCommand(CMD_EDIT_EDIT); return TRUE;
		case 'f': executeCommand(CMD_SEARCH_SEARCH); return TRUE;
		case 'g': executeCommand(CMD_SEARCH_SEARCH_NEXT); return TRUE;
		case 'h': executeCommand(CMD_EXPORT_KNOWDE_AS_HTML); return TRUE;
		case 'i': executeCommand(CMD_HELP_ABOUT); return TRUE;
		case 'n': executeCommand(CMD_FILE_NEW); return TRUE;
		case 'o': executeCommand(CMD_FILE_OPEN); return TRUE;
		case 'q': executeCommand(CMD_FILE_QUIT); return TRUE;
		case 's': executeCommand(CMD_FILE_SAVE); return TRUE;
		case 't': executeCommand(CMD_VIEW_TOGGLE_TREEVIEW); return TRUE;
		case 'v': executeCommand(CMD_EDIT_PASTE); return TRUE;
		case 'w': executeCommand(CMD_FILE_CLOSE); return TRUE;
		case 'x': executeCommand(CMD_EDIT_CUT); return TRUE;
		default:
			return FALSE;
	}
	return FALSE;
}

gboolean MainWindow::keypress_cb(GtkWidget *widget, GdkEventKey *event, gpointer data) {
//	printf("state=%i keyval=%i\n",event->state,event->keyval);
	if (data) 
		return ((MainWindow*) data)->keyPressed(event->state, event->keyval);
	return FALSE;
}

void MainWindow::clearClipboard() {
	if (!clipboard) return;
	delete clipboard;
	clipboard = NULL;
}

void MainWindow::executeCommand(Command command) {
	Knowde *kn; char buf[512];
	
	if ((command < 0) || (command > N_COMMANDS)) {
		printf ("Command %i is out of range!\n",command);
		return;
	}
	if (!activations[command]) {
//		printf("Command %i is not executable now!\n",command);
		return;
	}
	
	switch (command) {
		
		case CMD_FILE_NEW:
			if (!closeFile()) return;
			file = new KnowdeFile();
			if (!file)
				showErrorDialog(_("Could not create a new file!"));
			break;

		case CMD_FILE_OPEN:
			loadFile(NULL);
			break;
			
		case CMD_FILE_SAVE:
			saveFile(NULL);
			break;
			
		case CMD_FILE_SAVE_AS:
			if (!file) return;
			if (!showFileSelectionDialog(buf,512)) return;
			saveFile(buf);
			break;
		
		case CMD_FILE_CLOSE:
			closeFile();
			break;
		
		case CMD_FILE_QUIT:
			if (!closeFile()) break;
			quit();
			return;
		
		case CMD_EDIT_EDIT:
			kn = getSelectedKnowde();
			if (!kn) break;
			editor->show(kn);
			changed = TRUE;
			break;
		
		case CMD_EDIT_NEW_TOP_KNOWDE:
			kn = file->getData()->newChild();
			tree_view->insertTopKnowde(kn);
			list_view->insertTopKnowde(kn);
			time_view->insertTopKnowde(kn);
			selectKnowde(kn);
			editor->show(kn);
			changed = TRUE;
			break;
		
		case CMD_EDIT_NEW_SIBLING_KNOWDE:
			kn = getSelectedKnowde();
			if (!kn) break;
			kn = kn->getParent()->newChild();
			tree_view->insertSiblingKnowde(kn);
			list_view->insertSiblingKnowde(kn);
			time_view->insertSiblingKnowde(kn);
			selectKnowde(kn);
			editor->show(kn);
			changed = TRUE;
			break;
		
		case CMD_EDIT_NEW_SUB_KNOWDE:
			kn = getSelectedKnowde();
			if (!kn) break;
			kn = kn->newChild();
			tree_view->insertSubKnowde(kn);
			list_view->insertSubKnowde(kn);
			time_view->insertSubKnowde(kn);
			selectKnowde(kn);
			editor->show(kn);
			changed = TRUE;
			break;
		
		case CMD_VIEW_TOGGLE_TREEVIEW:
			switch (treeview) {
				case 0:
					gtk_widget_hide(tree_scroller);
					gtk_widget_show(list_scroller);
					break;
				case 1:
					gtk_widget_hide(list_scroller);
					gtk_widget_show(time_scroller);
					break;
				case 2:
					gtk_widget_hide(time_scroller);
					gtk_widget_show(tree_scroller);
					break;
				default: return;
			}
			treeview = ++treeview % 3;
			break;
		
		case CMD_EDIT_CUT:
			kn = getSelectedKnowde();
			
			// This used to be this:
			// file->removeKnowde(kn);
			// Now it is this:
			kn->unparent();
			// The knowde is lost in its relation to the file already now

			if (clipboard)
				clearClipboard();
			clipboard = kn;
			tree_view->removeKnowde(kn);
			list_view->removeKnowde(kn);
			time_view->removeKnowde(kn);
			changed = TRUE;
			break;

		case CMD_EDIT_COPY:
			if (getSelectedKnowde()) {
				if (clipboard)
					clearClipboard();
				kn = getSelectedKnowde()->clone();
				clipboard = kn;
			}
			break;

		case CMD_EDIT_PASTE:
			if (clipboard) {
				if (getSelectedKnowde()) {
					getSelectedKnowde()->adoptChild(clipboard);
					tree_view->insertSubKnowde(clipboard);
				} else
					tree_view->insertTopKnowde(clipboard);
				list_view->insertKnowde(clipboard);
				time_view->insertKnowde(clipboard);
				selectKnowde(clipboard);
				clipboard = NULL;
				changed = TRUE;
			}
			break;

		case CMD_EXPORT_KNOWDE_AS_HTML:
			if (getSelectedKnowde()) {
				if (!showFileSelectionDialog(buf,512)) break;
				ExportHTML::exportKnowde(getSelectedKnowde(),buf);
			}
			break;

		case CMD_EXPORT_FILE_AS_HTML:
			break;
			
		case CMD_SEARCH_SEARCH:
			last_found = 0;
			searcher->show();
			break;

		case CMD_SEARCH_SEARCH_NEXT:
//			printf("------------------ CMD_SEARCH_SEARCH_NEXT\n");
			if (last_found)
				executeSearch();
			break;
		
		case CMD_HELP_ABOUT:
			abouter->show();
			break;
	
		case CMD_HELP_HELP:
			unselectAll();
			reader->displayHelp();
			break;

		default:
			printf("Unknown command code %i\n",command);
			break;
	}
	updateActivations();
	updateTitle();
}

void MainWindow::updateKnowdeDisplay(Knowde *knowde) {
	if (!knowde) {
		reader->show(NULL);
		statusbar->show(NULL);
		unselectAll();
		return;
	}
	reader->show(knowde);
	statusbar->show(knowde);
}

void MainWindow::updateEditedKnowde(Knowde *knowde) {
	if (!knowde) return;
	tree_view->updateKnowde(knowde);
	list_view->updateKnowde(knowde);
	time_view->updateKnowde(knowde);

//	selectKnowde(knowde);
	reader->show(knowde);
	statusbar->show(knowde);
}

void MainWindow::updateActivations() {
	int c;
	for (c=0; c < N_COMMANDS; c++)
		activations[c] = FALSE;
		
	activations[CMD_FILE_NEW]=TRUE;
	activations[CMD_FILE_OPEN]=TRUE;
	activations[CMD_FILE_QUIT]=TRUE;
	activations[CMD_HELP_ABOUT]=TRUE;
	activations[CMD_HELP_HELP]=TRUE;
	
	if (!file) {
		menu->setActivations(activations);
		toolbar->setActivations(activations);
		return;
	}

	activations[CMD_FILE_SAVE] = TRUE;
	activations[CMD_FILE_SAVE_AS] = TRUE;
	activations[CMD_FILE_CLOSE] = TRUE;
	activations[CMD_SEARCH_SEARCH] = TRUE;
	activations[CMD_VIEW_TOGGLE_TREEVIEW] = TRUE;

	if (last_found)
		activations[CMD_SEARCH_SEARCH_NEXT] = TRUE;

	if (treeview==0)
		activations[CMD_EDIT_NEW_TOP_KNOWDE] = TRUE;
	
	if (changed)
		activations[CMD_FILE_SAVE] = TRUE;

	if (clipboard) activations[CMD_EDIT_PASTE] = TRUE;

	if (getSelectedKnowde())
		activations[CMD_EDIT_EDIT] = TRUE;
	
	if ((getSelectedKnowde()) && (treeview==0)) {
		activations[CMD_EDIT_NEW_SIBLING_KNOWDE] = TRUE;
		activations[CMD_EDIT_NEW_SUB_KNOWDE] = TRUE;
		// activations[CMD_EDIT_CREATE_REFERENCE] = TRUE;
		activations[CMD_EDIT_CUT] = TRUE;
		activations[CMD_EDIT_COPY] = TRUE;
		activations[CMD_EXPORT_KNOWDE_AS_HTML] = TRUE;
	}

	menu->setActivations(activations);
	toolbar->setActivations(activations);
}

Knowde* MainWindow::getSelectedKnowde() {
	if (!file) return NULL;
	return tree_view->getSelection();
/*	switch (treeview) {
		case 0: return tree_view->>getSelection();
		case 1: return list_view->>getSelection();
		case 2: return time_view->>getSelection();
		default: return NULL;
	}
*/
}

void MainWindow::selectKnowde(Knowde *knowde) {
//	printf("MainWindow::selectKnowde\n");
	if (tree_view->getSelection() != knowde) {
		tree_view->selectKnowde(knowde);
		list_view->selectKnowde(knowde);
		time_view->selectKnowde(knowde);
	}
	reader->show(knowde);
	statusbar->show(knowde);
	updateActivations();
}

void MainWindow::unselectAll() {
	tree_view->unselectAll();
	list_view->unselectAll();
	time_view->unselectAll();
	updateActivations();
}

void MainWindow::quit() {
	gtk_main_quit();
	exit(0);
}

void MainWindow::showPopupMenu(GtkWidget *widget) {
	gtk_menu_popup((GtkMenu*) menu->getPopupMenu(), NULL, NULL, NULL, widget, 0, 0);
}

void MainWindow::getWindowCoordinates(int *x, int *y, int *width, int *height) {
	GdkWindow *window = GDK_WINDOW(widget->window);
	gdk_window_get_position(window,x,y);
	*width = widget->allocation.width;
	*height = widget->allocation.height;
}

void MainWindow::centerWindow(GtkWidget *widget) {
	int my_x, my_y, my_w, my_h, x, y, h, w;
	if (!widget) return;
	getWindowCoordinates(&my_x, &my_y, &my_w, &my_h);
	w = widget->allocation.width;
	h = widget->allocation.height;
	w = w < 300 ? 300 : w;
	h = h < 150 ? 150 : h;
	x = my_x + (my_w/2) - (w/2);
	y = my_y + (my_h/2) - (h/2);
	gtk_widget_set_uposition(widget,x,y);
}

void MainWindow::executeSearch() {
	unselectAll();
	Knowde *kn = tree_view->search(last_found,
											searcher->getSearchExpression(),
											searcher->isTitle(),
											searcher->isDescription(),
											searcher->isText(),
											searcher->isKeywords(),
											searcher->isCaseSensitive());
	last_found = kn;
	if (kn) {
		selectKnowde(kn);
		updateKnowdeDisplay(kn);
	}
}

#endif

