#ifndef __TREE_VIEW_H__
#define __TREE_VIEW_H__

#include "Widget.h"
#include "KnowdeFile.h"
#include "Controller.h"

typedef enum {
	TITLE_COLUMN,
	DATA_COLUMN,
	PIX_COLUMN,
	PATH_COLUMN,
	N_COLUMNS
} Column;

typedef enum {
	TI_NONE,
	TI_SELECT,
	TI_REMOVE,
	TI_REMOVE_WITH_PARENT_IF_EMPTY,
	TI_PRINT,
	TI_UPDATE,
	TI_INSERT_SUB_KNOWDE,
	TI_INSERT_SIBLING_KNOWDE,
	N_TRAVERSE_INTENTIONS
} TraverseIntention;

typedef enum {
	TC_NONE,
	TC_KNOWDE_EQUALS,
	TC_TITLE_EQUALS,
	TC_HAS_NO_CHILDREN,
	TC_HAS_CHILDREN,
	N_TRAVERSE_CONDITIONS
} TraverseCondition;

class TreeView : public Widget {
	public:
		TreeView(Controller* _controller);
		virtual ~TreeView();
		virtual void insertTopKnowde(Knowde* knowde);
		virtual void insertSiblingKnowde(Knowde* knowde);
		virtual void insertSubKnowde(Knowde* knowde);
		virtual void insertKnowde(Knowde* knowde);
		virtual void removeKnowde(Knowde* knowde);
		virtual void selectKnowde(Knowde* knowde);
		virtual void updateKnowde(Knowde* knowde);
		Knowde* getSelection();
		Knowde* search(Knowde* after, char* expression, int title, int descr, int text, int keywords, int case_sensitive);
		void unselectAll();
		void clear();
		virtual void loadFile(KnowdeFile* file);
		int traverse(TraverseIntention intention, TraverseCondition condition, gpointer cond_data, gpointer exec_data, int multiple);
		GtkTreeModel* getTreeModel();
		
	protected:
		GtkTreeStore *model;
		GtkTreeIter iter;
		GtkTreeSelection *selection;
		GdkPixbuf *opened_pix;
		GdkPixbuf *closed_pix;
		GdkPixbuf *node_pix;
		int noUpdates;

		GtkTreeIter* find(TraverseCondition condition, Knowde *after, gpointer data);

	//	int  traverseFunction(TraverseIntention intention, TraverseCondition condition, GtkTreeIter *iter, Knowde *knowde, char *title);
		void insertKnowde(Knowde *knowde, GtkTreeIter *_iter = NULL);
		void removeKnowde(GtkTreeIter *_iter);
		
		void selection_changed();
		void button_pressed();
		void row_expanded(GtkTreeIter *iter);
		void row_collapsed(GtkTreeIter *iter);
		void row_activated(GtkTreePath *path);

		char* mystrstr(char* haystack, char* needle, int case_sensitive);
		
		static gboolean selection_changed_cb(GtkWidget *widget, gpointer data);
		static gboolean button_pressed_cb(GtkWidget *widget, GdkEventButton *event, gpointer data);
		static void row_collapsed_cb(GtkTreeView *treeview, GtkTreeIter *iter, GtkTreePath *path, gpointer data);
		static void row_expanded_cb(GtkTreeView *treeview, GtkTreeIter *iter, GtkTreePath *path, gpointer data);
		static void row_activated_cb(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *arg2, gpointer data);
//		static gint tree_iter_compare(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer data);
};

#endif

