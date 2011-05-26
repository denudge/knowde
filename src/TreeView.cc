#ifndef __TREE_VIEW_CC__
#define __TREE_VIEW_CC__

#include "TreeXpm.h"
#include "TreeView.h"

TraverseCondition tr_cond;
Knowde *tr_after;
Knowde *tr_knowde;
int tr_ready;
int tr_found;
GtkTreeIter tr_iter;

gint tree_iter_compare(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer data) {
	char *astr, *bstr; int res;
	gtk_tree_model_get(GTK_TREE_MODEL(model),a,TITLE_COLUMN,&astr,-1);
	if (!astr) return TRUE;
	gtk_tree_model_get(GTK_TREE_MODEL(model),b,TITLE_COLUMN,&bstr,-1);
	if (!bstr) { g_free(astr); return TRUE; }
	res = g_utf8_collate(astr,bstr);
	g_free(bstr); g_free(astr);
	return res;
}

gboolean tree_find_foreach(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data) {
	Knowde *tree_knowde = NULL; char *tree_title = NULL;

	gtk_tree_model_get(GTK_TREE_MODEL(model),iter,DATA_COLUMN,&tree_knowde,-1);
	if (!tree_knowde) return 0;

	if ((tr_after) && (tr_after==tree_knowde)) {
		tr_ready = 1;
		return 0;
	}
	if (!tr_ready) return 0;

	gtk_tree_model_get(GTK_TREE_MODEL(model),iter,TITLE_COLUMN,&tree_title,-1);
	if (!tree_title) return 0;
	
	switch (tr_cond) {
		case TC_NONE:
			tr_found = TRUE;
			break;
		case TC_KNOWDE_EQUALS:
			if (data == (gpointer) tree_knowde) tr_found = TRUE;
			break;
		case TC_TITLE_EQUALS:
			if (!strcmp((char*) data, tree_title)) tr_found = TRUE;
			break;
		default: break;
	}
	g_free(tree_title); tree_title = NULL;
	
	if (tr_found) {
		tr_knowde = tree_knowde;
		memcpy(&tr_iter,iter,sizeof(GtkTreeIter));
		return 1;
	} else return 0;
}

TreeView::TreeView(Controller *_controller) : Widget(_controller) {
	GtkCellRenderer *textrenderer, *pixrenderer;
	GtkTreeViewColumn *column;

	noUpdates = FALSE;

	model = gtk_tree_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_POINTER, GDK_TYPE_PIXBUF, G_TYPE_STRING);
	widget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
	
	textrenderer = gtk_cell_renderer_text_new();
	pixrenderer = gtk_cell_renderer_pixbuf_new();

	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column,_("Knowde Tree Browser"));
	gtk_tree_view_column_pack_start(column,pixrenderer,FALSE);
   gtk_tree_view_column_pack_start(column,textrenderer,FALSE);
	gtk_tree_view_column_set_attributes(column,pixrenderer,"pixbuf",PIX_COLUMN,NULL);
	gtk_tree_view_column_set_attributes(column,textrenderer,"text",TITLE_COLUMN,NULL);
	
	opened_pix = gdk_pixbuf_new_from_xpm_data(opened_xpm);
	closed_pix = gdk_pixbuf_new_from_xpm_data(closed_xpm);
	node_pix = gdk_pixbuf_new_from_xpm_data(node_xpm);
	
	gtk_tree_view_append_column(GTK_TREE_VIEW(widget), column);
	
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
	gtk_widget_add_events(widget, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
	
	g_signal_connect(G_OBJECT(gtk_tree_view_get_selection(GTK_TREE_VIEW(widget))),"changed",G_CALLBACK(TreeView::selection_changed_cb),this);
	g_signal_connect(G_OBJECT(widget),"button_press_event",G_CALLBACK(TreeView::button_pressed_cb),this);
	g_signal_connect(G_OBJECT(widget),"row-expanded",G_CALLBACK(TreeView::row_expanded_cb),this);
	g_signal_connect(G_OBJECT(widget),"row-collapsed",G_CALLBACK(TreeView::row_collapsed_cb),this);
	g_signal_connect(G_OBJECT(widget),"row-activated",G_CALLBACK(TreeView::row_activated_cb),this);

	gtk_tree_sortable_set_sort_func((GtkTreeSortable*) model,TITLE_COLUMN,(*tree_iter_compare),this,NULL);
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable*) model, TITLE_COLUMN, GTK_SORT_ASCENDING);
}

TreeView::~TreeView() {
//	gtk_widget_hide(widget);
//	delete widget;
}

GtkTreeModel* TreeView::getTreeModel() {
	return GTK_TREE_MODEL(model);
}

void TreeView::insertKnowde(Knowde *knowde, GtkTreeIter *_iter) {
	GtkTreeIter iter, niter; int c, children;

//	printf("TreeView::insertKnowde(Knowde*,*iter)\n");
	if (!knowde) return;
	
	if (_iter) {
		memcpy(&iter,_iter,sizeof(GtkTreeIter));
	}
	else
		gtk_tree_store_append(model,&iter,NULL);

	children = knowde->countChildren();
	gtk_tree_store_set(model,&iter,
									TITLE_COLUMN, knowde->getTitle(),
									DATA_COLUMN,  (gpointer) knowde,
									PIX_COLUMN,   (children ? closed_pix : node_pix),
							-1);

	for (c=0; c < children; c++) {
		gtk_tree_store_append(model, &niter, &iter);
		insertKnowde(knowde->getChild(c),&niter);
	}
}

void TreeView::insertTopKnowde(Knowde *knowde) {
	insertKnowde(knowde,NULL);
}

void TreeView::insertKnowde(Knowde *knowde) {
	int c;
	Knowde *parent;
//	printf("TreeView::insertKnowde(Knowde*)\n");

	if (!knowde) return;
	parent = knowde->getParent();

	if ((!parent) || (!parent->getParent())) {
		insertKnowde(knowde, NULL);
	} else {
		c = traverse(TI_INSERT_SUB_KNOWDE, TC_KNOWDE_EQUALS, parent, knowde, FALSE);
		// printf("knowde \"%s\" was inserted %i times.\n",knowde->getTitle(),c);
	}
}

void TreeView::insertSiblingKnowde(Knowde *knowde) {
	GtkTreeIter iter, niter;
	if (!gtk_tree_selection_get_selected(selection, (GtkTreeModel**) &model, &iter)) return;
	gtk_tree_store_insert_before(model,&niter,NULL,&iter);
	insertKnowde(knowde,&niter);
	gtk_tree_selection_select_iter(selection,&niter);
}

void TreeView::insertSubKnowde(Knowde *knowde) {
	GtkTreeIter iter, niter;
	if (!gtk_tree_selection_get_selected(selection, (GtkTreeModel**) &model, &iter)) return;
	gtk_tree_store_append(model,&niter,&iter);
	insertKnowde(knowde,&niter);
	gtk_tree_view_expand_row(GTK_TREE_VIEW(widget), gtk_tree_model_get_path(GTK_TREE_MODEL(model), &iter),TRUE);
	gtk_tree_selection_select_iter(selection,&niter);
}

void TreeView::loadFile(KnowdeFile *file) {
	int c, s; Knowde *knowde;
	if (!file) return;
	noUpdates = TRUE;
	knowde = file->getData();
	s = knowde->countChildren();
	for (c=0;c<s;c++)
		insertTopKnowde(knowde->getChild(c));
	noUpdates = FALSE;
}

void TreeView::removeKnowde(GtkTreeIter *_iter) {
	GtkTreeIter parent; // , iter;
	int p; // , s, c;
	GtkTreePath *ppath = NULL; //, *path = NULL;
	
	if (!_iter) return;
	
	// also do this for the parent, if available
	p = gtk_tree_model_iter_parent(GTK_TREE_MODEL(model),&parent,_iter);
	if (p)
		ppath = gtk_tree_model_get_path(GTK_TREE_MODEL(model),&parent);
	
	gtk_tree_store_remove(model,_iter);
	
	// if we have to update the parent, we do it
	if (ppath) gtk_tree_model_get_iter(GTK_TREE_MODEL(model),&parent,ppath);
	if ((ppath) && (!gtk_tree_model_iter_n_children(GTK_TREE_MODEL(model),&parent)))
		gtk_tree_store_set(model,&parent,PIX_COLUMN,node_pix,-1);
}

void TreeView::removeKnowde(Knowde *knowde) {
	int c, children;
	children = knowde->countChildren();
	for (c=children-1; c>=0;c--)
		traverse(TI_REMOVE, TC_KNOWDE_EQUALS, knowde->getChild(c), NULL, TRUE);
	traverse(TI_REMOVE, TC_KNOWDE_EQUALS, knowde, NULL, TRUE);
}

void TreeView::selectKnowde(Knowde *knowde) {
//	printf("TreeView::selectKnowde\n");
	if (!knowde) return;
	if (noUpdates) return;
	traverse(TI_SELECT,TC_KNOWDE_EQUALS, knowde, NULL, TRUE);
}

void TreeView::unselectAll() {
	gtk_tree_selection_unselect_all(selection);
}

void TreeView::clear() {
	gtk_tree_store_clear(model);
}

Knowde* TreeView::getSelection() {
	GtkTreeIter iter; Knowde *knowde;
	if (!gtk_tree_selection_get_selected(selection, (GtkTreeModel**) &model, &iter)) return NULL;
	gtk_tree_model_get(GTK_TREE_MODEL(model),&iter,DATA_COLUMN,&knowde,-1);
	return knowde;
}

char* TreeView::mystrstr(char* haystack, char* needle, int case_sensitive) {
	int i;
	int nlength = strlen (needle); 
	int hlength = strlen (haystack);

//	 printf("haystack length=%i, needle length=%i\n",hlength,nlength);
	if (nlength > hlength) return NULL;
	if (hlength <= 0) return NULL;
	if (nlength <= 0) return haystack;
	
	// hlength and nlength > 0, nlength <= hlength
	for (i = 0; i <= (hlength - nlength); i++) {
		if ((case_sensitive ? strncmp (haystack + i, needle, nlength) : strncasecmp (haystack + i, needle, nlength)) == 0) {
			return haystack + i;
		}
	} 
	// substring not found
	return NULL;
}

Knowde* TreeView::search(Knowde* after, char* expression, int title, int descr, int text, int keywords, int case_sensitive) {
	Knowde* knowde = NULL; // int ready = 0;
	GtkTreeIter *result; // int counter = 0;
	
	knowde = after;
	while ((result = find(TC_NONE, knowde, NULL)) != NULL) {
		// counter++;
		// if (counter>20) break;
		gtk_tree_model_get(GTK_TREE_MODEL(model), result, DATA_COLUMN, &knowde, -1);
		if (!knowde) continue;
		if ((title) && (mystrstr(knowde->getTitle(),expression, case_sensitive))) { return knowde; }
		if ((descr) && (mystrstr(knowde->getDescr(),expression, case_sensitive))) { return knowde; }
		if ((text) && (mystrstr(knowde->getText(),expression, case_sensitive))) { return knowde; }
		if ((keywords) && (mystrstr(knowde->getKeywords(),expression, case_sensitive))) { return knowde; }
	}
	return NULL;
}

void TreeView::updateKnowde(Knowde *knowde) {
	if (noUpdates) return;
	if (!knowde) return;
//	printf("Tree updates knowde entry\n");
	traverse(TI_UPDATE, TC_KNOWDE_EQUALS, knowde, NULL, FALSE);
}

void TreeView::selection_changed() {
	Knowde *knowde;
	// auf der rechten Seite alles aktualisieren
	// ...	
	knowde = getSelection();
	if (!knowde) return;
	
	noUpdates = TRUE;
	if (controller)
		controller->selectKnowde(knowde);
	noUpdates = FALSE;
}

int TreeView::traverse(TraverseIntention intention, TraverseCondition condition, gpointer cond_data, gpointer exec_data, int multiple) {
	GtkTreeIter *iter, parent; GtkTreePath *path = NULL;
	Knowde *knowde = NULL, *oldknowde = NULL; 
	int counter = 0, c, knowde_counter = 0;
	
	while ((iter = find(condition,knowde,cond_data)) != NULL) {
		if (knowde_counter > 20) { 
			printf("TreeView::traverse() : Endless Loop?\n");
			return counter;
		}
		// if (knowde) printf("I have searched with knowde \"%s\" as activation point.\n",knowde->getTitle());
		gtk_tree_model_get(GTK_TREE_MODEL(model),iter,DATA_COLUMN,&knowde,-1);
//		printf("Knowde \"%s\" is the one.\n",(knowde?knowde->getTitle():"NULL"));

		counter++;
		
		if (knowde==oldknowde) knowde_counter++;
		else {
			knowde_counter=1;
			oldknowde = knowde;
		}

		switch(intention) {
			
			case TI_NONE:
				break;
			
			case TI_UPDATE:
				gtk_tree_store_set(model,iter,TITLE_COLUMN,knowde->getTitle(),-1);
				break;
			
			case TI_REMOVE:
				removeKnowde(iter);
				if (multiple) knowde = NULL;
				break;
			
			case TI_REMOVE_WITH_PARENT_IF_EMPTY:
//				printf("TI_Remove_with_parent_if_empty called.\n");
				c = gtk_tree_model_iter_parent(GTK_TREE_MODEL(model),&parent,iter);
				if (c) path = gtk_tree_model_get_path(GTK_TREE_MODEL(model),&parent);
				removeKnowde(iter);
				if (c) {
					gtk_tree_model_get_iter(GTK_TREE_MODEL(model),&parent,path);
					gtk_tree_path_free(path);
					if (!gtk_tree_model_iter_n_children(GTK_TREE_MODEL(model),&parent))
						removeKnowde(&parent);
				}
				if (multiple) knowde = NULL;
				break;

			case TI_INSERT_SUB_KNOWDE:
				if (!exec_data) break;
				gtk_tree_store_append(model,&parent,iter);
				insertKnowde((Knowde*) exec_data,&parent);
				break;
			
			case TI_SELECT:
				
				if (gtk_tree_model_iter_parent(GTK_TREE_MODEL(model),&parent,iter)) {
					path = gtk_tree_model_get_path(GTK_TREE_MODEL(model),&parent);
					gtk_tree_view_expand_to_path(GTK_TREE_VIEW(widget),path);
					g_free(path);
				}
				gtk_tree_selection_select_iter(selection,iter);
				break;
				
			default:
				break;
		}
		// printf("Counter: %i\n",counter);
		if (!multiple) {
			return counter;
		}
	}
	return counter;
}

GtkTreeIter* TreeView::find(TraverseCondition condition, Knowde *after, gpointer data) {
	tr_cond = condition;
	tr_found = 0;
	if (after) {
		tr_ready = 0;
		tr_after = after;
	} else {
		tr_ready = 1;
		tr_after = NULL;
	}
	gtk_tree_model_foreach(GTK_TREE_MODEL(model),(tree_find_foreach),data);
	if (tr_found)
		return &tr_iter;
	else
		return NULL;
}

gboolean TreeView::selection_changed_cb(GtkWidget *widget, gpointer data) {
	if (data) ((TreeView*) data)->selection_changed();
	return TRUE;
}

void TreeView::button_pressed() {
	controller->showPopupMenu(widget);
}

gboolean TreeView::button_pressed_cb(GtkWidget *widget, GdkEventButton *event, gpointer data) {
	if (event->button==1) return FALSE;

	if (data) ((TreeView*) data)->button_pressed();
	return FALSE;
}

void TreeView::row_expanded_cb(GtkTreeView *treeview, GtkTreeIter *iter, GtkTreePath *path, gpointer data) {
	if (data) ((TreeView*) data)->row_expanded(iter);
}

void TreeView::row_collapsed_cb(GtkTreeView *treeview, GtkTreeIter *iter, GtkTreePath *path, gpointer data) {
	if (data) ((TreeView*) data)->row_collapsed(iter);
}

void TreeView::row_expanded(GtkTreeIter *iter) {
	gtk_tree_store_set(model,iter,PIX_COLUMN,opened_pix,-1);
}

void TreeView::row_collapsed(GtkTreeIter *iter) {
	gtk_tree_store_set(model,iter,PIX_COLUMN,closed_pix,-1);
}

void TreeView::row_activated_cb(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *arg2, gpointer data) {
	if (data) ((TreeView*) data)->row_activated(path);
}

void TreeView::row_activated(GtkTreePath *path) {
	// GtkTreeIter *iter;
	if (controller)
		controller->executeCommand(CMD_EDIT_EDIT);
}

#endif

