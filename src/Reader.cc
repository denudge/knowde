#ifndef __READER_CC__
#define __READER_CC__

#include "Reader.h"
#include "Help.h"

Reader::Reader(Controller *_controller) : Widget(_controller) {
	title_label = gtk_label_new(_("Title: "));
	descr_label = gtk_label_new(_("Description: "));
	
	label_box = gtk_vbox_new(TRUE,0);
	gtk_box_pack_start(GTK_BOX(label_box),title_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(label_box),descr_label,FALSE,FALSE,0);
	
	keyword_tree = NULL;
	title = gtk_label_new("");
	descr = gtk_label_new("");

	entry_box = gtk_vbox_new(TRUE,0);
	gtk_box_pack_start(GTK_BOX(entry_box),title,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(entry_box),descr,TRUE,TRUE,0);
	
	gtk_label_set_justify(GTK_LABEL(title_label),GTK_JUSTIFY_RIGHT);
	gtk_label_set_justify(GTK_LABEL(descr_label),GTK_JUSTIFY_RIGHT);
	gtk_label_set_justify(GTK_LABEL(title),GTK_JUSTIFY_LEFT);
	gtk_label_set_justify(GTK_LABEL(descr),GTK_JUSTIFY_LEFT);

	header_box = gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(header_box),entry_box,TRUE,TRUE,0);

	header_frame = gtk_frame_new(_("Meta Information"));
	gtk_container_add(GTK_CONTAINER(header_frame),header_box);

	date_label = gtk_label_new(_("Date: "));
	gtk_widget_set_sensitive(date_label, FALSE);
	
	key_choice = gtk_option_menu_new();
	GtkWidget *menu = gtk_menu_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_menu_item_new_with_label(" -- Keywords -- "));
	gtk_option_menu_set_menu(GTK_OPTION_MENU(key_choice), menu);
	gtk_widget_set_sensitive(key_choice, FALSE);

	key_box = gtk_hbox_new(FALSE,5);
	gtk_box_pack_start(GTK_BOX(key_box),date_label,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(key_box),key_choice,FALSE,FALSE,0);
	
	text = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(text),FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text), GTK_WRAP_WORD);
	
	text_scroller = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_add_with_viewport((GtkScrolledWindow*) text_scroller,text);
	gtk_scrolled_window_set_policy((GtkScrolledWindow*) text_scroller, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	text_frame = gtk_frame_new(_("Text: "));
	gtk_container_add(GTK_CONTAINER(text_frame),text_scroller);

	widget = gtk_vbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(widget),header_frame,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(widget),key_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(widget),text_frame,TRUE,TRUE,0);
	gtk_widget_show_all(widget);
}

Reader::~Reader() {
}
		
void Reader::setKeywordTree(GtkTreeModel *tree_model) {
	keyword_tree = tree_model;
}

void Reader::clear() {
	GtkWidget *menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(key_choice));
	gtk_widget_destroy(menu);
	menu = gtk_menu_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),gtk_menu_item_new_with_label(" -- Keywords -- "));
	gtk_option_menu_set_menu(GTK_OPTION_MENU(key_choice), menu);
	gtk_widget_set_sensitive(key_choice, FALSE);
	gtk_label_set_text(GTK_LABEL(title),"");
	gtk_label_set_text(GTK_LABEL(descr),"");
	gtk_label_set_text(GTK_LABEL(date_label),"Date: ");
	gtk_widget_set_sensitive(date_label, FALSE);
	gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text)),"",0);
	gtk_widget_modify_font(text, NULL);
	gtk_widget_modify_base(text, GTK_STATE_NORMAL, NULL);
	gtk_widget_modify_text(text, GTK_STATE_NORMAL, NULL);
}

void Reader::displayHelp() {
	clear();
	PangoFontDescription *font_desc = pango_font_description_from_string ("Arial Bold, 12");
	gtk_widget_modify_font(text, font_desc);
	pango_font_description_free(font_desc);
	gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text)),help,strlen(help));
	
}

void Reader::fillKeywordList(Knowde *knowde) {
	char *keywords, *word, *entry; int c, n_top, n_children, child, top;
	GtkWidget *item; GtkTreeIter top_iter, child_iter; Knowde *entry_knowde;
	char category[512];

	GtkWidget *menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(key_choice));
	gtk_widget_destroy(menu);
	menu = gtk_menu_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),gtk_menu_item_new_with_label(" -- Keywords -- "));
	
	if ((!knowde) || (!keyword_tree)) {
		gtk_option_menu_set_menu(GTK_OPTION_MENU(key_choice), menu);
		gtk_widget_set_sensitive(key_choice, FALSE);
		return;
	}	
//	gtk_menu_shell_append(GTK_MENU_SHELL(menu),gtk_menu_item_new_with_label(" "));	
	
	word = knowde->getKeywords();
	if (!word[0]) {
	//	printf("No keywords!\n");
		gtk_option_menu_set_menu(GTK_OPTION_MENU(key_choice), menu);
		gtk_widget_set_sensitive(key_choice, FALSE);
		return;
	}
	c = strlen(word);
	
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
		
//		printf("Keyword: '%s'\n",word);

		n_top = gtk_tree_model_iter_n_children(keyword_tree,NULL);
		for (top = 0; top < n_top; top++) {
			if (!gtk_tree_model_iter_nth_child(keyword_tree, &top_iter, NULL, top)) continue;
			gtk_tree_model_get(keyword_tree, &top_iter, 0, &entry, -1);
			if (!entry) continue;
			if (!strcmp(word,entry)) {
				free(entry); entry = 0;
				
				// exclude the knowde itself
				n_children = gtk_tree_model_iter_n_children(keyword_tree,&top_iter);
				if (n_children < 2) continue;
			
				// insert category	
				strcpy(category,""); strncat(category,word,30); strcat(category,": ");
				gtk_menu_shell_append(GTK_MENU_SHELL(menu),gtk_menu_item_new_with_label(" "));	
				gtk_menu_shell_append(GTK_MENU_SHELL(menu),gtk_menu_item_new_with_label(category));	
				
				// insert other knowdes
				for (child = 0; child < n_children; child++) {
					if (!gtk_tree_model_iter_nth_child(keyword_tree,&child_iter, &top_iter, child)) continue;
					gtk_tree_model_get(keyword_tree, &child_iter, 1, &entry_knowde, -1);
					if ((!entry_knowde) || (knowde == entry_knowde)) continue;
					
					strcpy(category, " -> "); strncat(category,entry_knowde->getTitle(),30);
					item = gtk_menu_item_new_with_label(category);
					g_object_set_data(G_OBJECT(item),"knowde",(gpointer) entry_knowde);
					g_signal_connect(G_OBJECT (item),"activate",G_CALLBACK(Reader::keyword_cb), this);
					gtk_menu_shell_append(GTK_MENU_SHELL(menu),item);
				}
			}
		}
		word = strtok(NULL,",");
	}
	gtk_option_menu_set_menu(GTK_OPTION_MENU(key_choice), menu);
	gtk_widget_set_sensitive(key_choice, TRUE);
	gtk_widget_show_all(key_choice);
	free(keywords);
}

void Reader::show(Knowde *knowde) {
	char datebuf[64];

	gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text)),"",0);
	if (!knowde) return;
	gtk_label_set_text(GTK_LABEL(title),(gchar*) knowde->getTitle());
	gtk_label_set_text(GTK_LABEL(descr),(gchar*) knowde->getDescr());
	
	// changing font
	gtk_widget_modify_font(text, NULL);
	if (knowde->hasTextFont()) {
		PangoFontDescription *font_desc = pango_font_description_from_string (knowde->getTextFont());
		gtk_widget_modify_font(text, font_desc);
		pango_font_description_free(font_desc);
	} else if ((controller) && (controller->getCurrentKnowdeFile()) && (controller->getCurrentKnowdeFile()->hasDefaultTextFont())) {
		PangoFontDescription *font_desc = pango_font_description_from_string(controller->getCurrentKnowdeFile()->getDefaultTextFont());
		gtk_widget_modify_font(text, font_desc);
		pango_font_description_free(font_desc);
	}

	// changing background and text color
	gtk_widget_modify_base(text, GTK_STATE_NORMAL, NULL);
	if (knowde->hasBackgroundColor())
		gtk_widget_modify_base(text, GTK_STATE_NORMAL, knowde->getBackgroundColor());
	
	gtk_widget_modify_text(text, GTK_STATE_NORMAL, NULL);
	if (knowde->hasTextColor())
		gtk_widget_modify_text(text, GTK_STATE_NORMAL, knowde->getTextColor());

	// setting the content
	strcpy(datebuf,"Date: ");
	gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text)),knowde->getText(),strlen(knowde->getText()));
	if (knowde->getStartDate()) {
		strcat(datebuf,knowde->getStartDate()->getString());
		if (knowde->getEndDate()) {
			strcat(datebuf," - ");
			strcat(datebuf,knowde->getEndDate()->getString());
		}
		gtk_label_set_text(GTK_LABEL(date_label), datebuf);
		gtk_widget_set_sensitive(date_label, TRUE);
	} else {
		gtk_label_set_text(GTK_LABEL(date_label),"Date: ");
		gtk_widget_set_sensitive(date_label, FALSE);
	}
	
	fillKeywordList(knowde);
}

void Reader::keyword_cb(GtkMenuItem *item, gpointer data) {
	if (data) ((Reader*) data)->selectKnowde((Knowde*) g_object_get_data(G_OBJECT(item),"knowde"));
}

void Reader::selectKnowde(Knowde *knowde) {
	if (controller)
		controller->selectKnowde(knowde);
}

#endif

