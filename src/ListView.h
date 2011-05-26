#ifndef __LIST_VIEW_H__
#define __LIST_VIEW_H__

#include "TreeView.h"

class ListView : public TreeView {
	public:
		ListView(Controller *_controller);
		virtual ~ListView();
		virtual void insertTopKnowde(Knowde *knowde);
		virtual void insertSiblingKnowde(Knowde *knowde);
		virtual void insertSubKnowde(Knowde *knowde);
		
		virtual void insertKnowde(Knowde *knowde);
		virtual void removeKnowde(Knowde *knowde);
		virtual void updateKnowde(Knowde *knowde);
		virtual void loadFile(KnowdeFile *file);

	protected:
		virtual void insertKnowde(Knowde *knowde, GtkTreeIter *iter);
		int insertKnowdeUnderTitle(char *title, Knowde *knowde);

};

#endif

