#ifndef __LIST_VIEW_CC__
#define __LIST_VIEW_CC__

#include "ListView.h"

ListView::ListView(Controller *_controller) : TreeView(_controller) {
	GtkTreeIter iter;

	gtk_tree_store_append(model,&iter,NULL);
	gtk_tree_store_set(model,&iter,TITLE_COLUMN,_("Unreferenced"),
																	PIX_COLUMN,closed_pix,-1);
	gtk_tree_view_column_set_title(gtk_tree_view_get_column(GTK_TREE_VIEW(widget),0),_("Keyword Browser"));
}

ListView::~ListView() { }

void ListView::insertKnowde(Knowde *knowde, GtkTreeIter *_iter) {
	int c, children;
		
//	printf("ListView:insertKnowde(Knowde*,*iter)\n");
	if (!knowde) return;
	
	gtk_tree_store_set(model,_iter,TITLE_COLUMN,knowde->getTitle(),
											DATA_COLUMN,knowde,
											PIX_COLUMN,node_pix,
											-1);

	children = knowde->countChildren();
	for (c=0 ; c < children ; c++)
		insertKnowde(knowde->getChild(c));
}


int ListView::insertKnowdeUnderTitle(char *title, Knowde *knowde) {
	GtkTreeIter iter, child; char *tree_title;
	
	if (!knowde) return 0;
	
	if (! gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter)) {
		gtk_tree_store_append(model,&iter,NULL);
		gtk_tree_store_set(model,&iter,TITLE_COLUMN,title,PIX_COLUMN,opened_pix,-1);
		gtk_tree_store_append(model,&child,&iter);
		insertKnowde(knowde,&child);
		return 1;
	}
	
	gtk_tree_model_get(GTK_TREE_MODEL(model),&iter,TITLE_COLUMN,&tree_title,-1);
	if (!strcmp(title,tree_title)) {
		gtk_tree_store_append(model,&child,&iter);
		insertKnowde(knowde,&child);
		g_free(tree_title);
		return 1;
	}
	g_free(tree_title);

	while (gtk_tree_model_iter_next(GTK_TREE_MODEL(model),&iter)) {
		gtk_tree_model_get(GTK_TREE_MODEL(model),&iter,TITLE_COLUMN,&tree_title,-1);
		if (!strcmp(title,tree_title)) {
			gtk_tree_store_append(model,&child,&iter);
			insertKnowde(knowde,&child);
			g_free(tree_title);
			return 1;
		}
		g_free(tree_title);
	}

	gtk_tree_store_append(model,&iter,NULL);
	gtk_tree_store_set(model,&iter,TITLE_COLUMN,title,PIX_COLUMN,opened_pix,-1);
	gtk_tree_store_append(model,&child,&iter);
	insertKnowde(knowde,&child);
	return 1;
}

void ListView::insertKnowde(Knowde *knowde) {
	char *keywords, *word; int c;
	// GtkTreeIter iter, niter;
	char unref[256];

	strcpy(unref,_("Unreferenced"));
	
//	printf("ListView::insertKnowde(Knowde*)\n");
	if (!knowde) return;

	word = knowde->getKeywords();
	c = strlen(word);
//	printf("Inserting in keywords: \"%s\" (%i chars)\n",word,c);
	if (!c) { 
		word = unref;
		c = strlen(word);
	}

	keywords = (char*) malloc(c+1);
	strcpy(keywords,word);
	word = strtok(keywords,",");
	while (word) {
		
		// trimming the word
		while (word[0]==' ') word++;
		c = strlen(word);
		while ((c>0) && (word[c-1]==' ')) { word[c-1]=0;c--; }

		// throw out empty words		
		if (!word[0]) { word = strtok(NULL,","); continue; }
		
		insertKnowdeUnderTitle(word, knowde);
		word = strtok(NULL,",");
	}
	free(keywords); keywords = NULL;
}

void ListView::loadFile(KnowdeFile *file) {
	int c = file->getData()->countChildren();
	noUpdates = TRUE;
	for (c=c-1;c>=0;c--)
		insertKnowde(file->getData()->getChild(c));
	noUpdates = FALSE;
}

void ListView::insertTopKnowde(Knowde *knowde) {
	insertKnowde(knowde);
}

void ListView::insertSiblingKnowde(Knowde *knowde) {
	insertKnowde(knowde);
}

void ListView::insertSubKnowde(Knowde *knowde) {
	insertKnowde(knowde);
}

void ListView::removeKnowde(Knowde *knowde) {
	int c; int children, s = 0;
//	printf("ListView::removeKnowde(Knowde*)\n");
	if (!knowde) return;
	children = knowde->countChildren();
	for (c = children-1; c>=0; c--)
		s = traverse(TI_REMOVE_WITH_PARENT_IF_EMPTY, TC_KNOWDE_EQUALS, knowde->getChild(c), NULL, TRUE);
	s += traverse(TI_REMOVE_WITH_PARENT_IF_EMPTY, TC_KNOWDE_EQUALS, knowde, NULL, TRUE);
//	printf("Knowde \"%s\" was removed %i times.\n",knowde->getTitle(),s);
}

void ListView::updateKnowde(Knowde *knowde) {
	if (noUpdates) return;
	if (!knowde) return;
//	printf("ListView::updateKnowde(Knowde*)\n");
	removeKnowde(knowde);
	insertKnowde(knowde);
}

#endif

