#ifndef __TIME_VIEW_H__
#define __TIME_VIEW_H__

#include "ListView.h"

class TimeView : public ListView {
	public:
		TimeView(Controller *_controller);
		virtual ~TimeView();
		virtual void insertKnowde(Knowde *knowde);

	public:
		GdkPixbuf *point_pix;
		virtual void insertKnowde(Knowde *knowde, GtkTreeIter *_iter);
};

#endif

