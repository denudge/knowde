#ifndef __TIME_VIEW_CC__
#define __TIME_VIEW_CC__

#include "TimeView.h"
#include "TimeXpm.h"

char treeinserttitlebuf[512];
char treeinsertpathbuf[4096];

gint tree_iter_compare2(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer data) {
	long idx_a, idx_b; Knowde *ak, *bk;

	gtk_tree_model_get(GTK_TREE_MODEL(model),a,DATA_COLUMN,&ak,-1);
	if (!ak) return 1;
	gtk_tree_model_get(GTK_TREE_MODEL(model),b,DATA_COLUMN,&bk,-1);
	if (!bk) return -1;
	
	if (!ak->hasDate()) return 1;
	if (!bk->hasDate()) return -1;
	idx_a = ak->getStartDate()->getValue();
	idx_b = bk->getStartDate()->getValue();
	if (idx_a < idx_b) return -1;
	if (idx_a > idx_b) return 1;

	if (!ak->isDateDurative()) return 1;
	if (!bk->isDateDurative()) return -1;

	idx_a = ak->getEndDate()->getValue();
	idx_b = bk->getEndDate()->getValue();
	
	if (idx_a < idx_b) return 1;
	if (idx_a > idx_b) return -1;
	
	return 0;
}

TimeView::TimeView(Controller *_controller) : ListView(_controller) {
	GtkTreeViewColumn *column;
	GtkCellRenderer *textrenderer;

	textrenderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new();	
	gtk_tree_view_column_set_title(column,_("Tree Path"));
   gtk_tree_view_column_pack_start(column,textrenderer,FALSE);
	gtk_tree_view_column_set_attributes(column,textrenderer,"text",PATH_COLUMN,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(widget), column);
	
	point_pix = gdk_pixbuf_new_from_xpm_data(point_xpm);
	gtk_tree_view_column_set_title(gtk_tree_view_get_column(GTK_TREE_VIEW(widget),0),_("Time Viewer"));
	gtk_tree_sortable_set_sort_func((GtkTreeSortable*) model,TITLE_COLUMN,(*tree_iter_compare2),this,NULL);
	gtk_tree_sortable_set_sort_column_id((GtkTreeSortable*) model, TITLE_COLUMN, GTK_SORT_ASCENDING);
	clear();
}

TimeView::~TimeView() { }

void TimeView::insertKnowde(Knowde *knowde, GtkTreeIter *_iter) { }

void TimeView::insertKnowde(Knowde *knowde) {
	GtkTreeIter iter;
	int c;
	
	if (!knowde) return;
	if (knowde->hasDate()) {
		gtk_tree_store_append(model,&iter,NULL);
		sprintf(treeinserttitlebuf,"%s: %s",knowde->getDate(),knowde->getTitle());
	
		knowde->getPathString(treeinsertpathbuf);
/*		if ((knowde->getParent()) && (knowde->getParent()->getParent())) {
			strcat(buf," \t\t(");
			strcat(buf,knowde->getParent()->getTitle());
			strcat(buf,")");
		}
*/		gtk_tree_store_set(model,&iter,TITLE_COLUMN,treeinserttitlebuf,
												PATH_COLUMN,treeinsertpathbuf,
												PIX_COLUMN,point_pix,
												DATA_COLUMN,(gpointer) knowde,-1);
	}
	for (c=0;c<knowde->countChildren();c++)
		insertKnowde(knowde->getChild(c));
}

#endif

