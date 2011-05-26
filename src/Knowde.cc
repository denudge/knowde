#ifndef __KNOWDE_CC__
#define __KNOWDE_CC__

#include <time.h>
#include <string.h>
#include "Knowde.h"
#include "config.h"

static int elem_counter;

Knowde::Knowde(Knowde* _parent, char* _hash) {
	time_t mytime;

	time(&mytime);
	if ((_hash) && (_hash[0]))
		strncpy(hash,_hash,63);
	else
		sprintf(hash,"KNWD%i_%i",(int) mytime,++elem_counter);
	title 		= g_string_new(_("[noname]"));
	descr 		= g_string_new("");
	text 			= g_string_new("");
	date 			= g_string_new("");
	parent 		= _parent;
	keywords 	= g_string_new("");
	start_date 	= NULL;
	end_date 	= NULL;
	text_font 	= NULL;
	bg_color		= NULL;
	text_color 	= NULL;
}

Knowde::~Knowde() {
	Knowde *child;
	g_string_free(text,TRUE);
	g_string_free(keywords, TRUE);
	g_string_free(title, TRUE);
	g_string_free(descr, TRUE);
	g_string_free(date, TRUE);
	
	parent = NULL;
	// keywords.clear();
	while (!children.empty()) {
		child = children.back();
		children.pop_back();
		delete child;
	}

	if (start_date) { delete start_date; start_date = NULL; }
	if (end_date) { delete end_date; end_date = NULL; }
	if (text_font) { g_string_free(text_font, TRUE); text_font = NULL; }
	if (text_color) { free(text_color); text_color = NULL; }
	if (bg_color) { free(bg_color); bg_color = NULL; }

	//	printf("Deleted %s\n",hash);
}

void Knowde::print() {
	printf("--\nHash:  %s\n",hash);
	printf("Titel: %s\n",title->str);
	printf("Descr: %s\n",descr->str);
	printf("Text:  %s\n",text->str);
	if (parent)
		printf("Parent: %s\n--\n",parent->getHash());
}

void Knowde::getPathString(char *dest) {
	if ((!parent) || (!parent->getParent())) {
		dest[0]=0;
		return;
	} else parent->getPathString(dest);
	if (dest[0]) strcat(dest," / ");
	strcat(dest,parent->getTitle());
	return;
}

void Knowde::adoptChild(Knowde *child) {
	child->reparent(this);
	children.push_back(child);
}

void Knowde::unparent() {
	unsigned int c, children;
	
	if (!parent) return;
	
	children = parent->countChildren();
	for (c=0;c<children;c++)
		if (parent->getChild(c)==this)
		{ parent->cutChild(c); break; }
	parent = NULL;
}

void Knowde::reparent(Knowde *newParent) {
	if (!newParent) return;
	unparent();
	parent = newParent;
}

Knowde* Knowde::cutChild(unsigned int child_index) {
	std::vector<Knowde*>::iterator iter;
	Knowde *knowde = getChild(child_index);
	if (knowde) {
		iter = children.begin();
		children.erase(iter+child_index);
		knowde->unparent();
		return knowde;
	} else return NULL;
}

Knowde* Knowde::clone() {
	Knowde *kn; int children, c;
	
	kn = new Knowde(NULL, NULL);
	kn->setTitle(getTitle());
	kn->setDescr(getDescr());
	kn->setText(getText());
	kn->setKeywords(getKeywords());
	
	children = countChildren();
	for (c=0;c<children;c++)
		kn->adoptChild(getChild(c)->clone());
	
	// new: date cloning
	if (start_date) {
		kn->enableDate();
		kn->getStartDate()->fromString(start_date->getString());
		if (end_date) {
			kn->setDateDurative(TRUE);
			kn->getEndDate()->fromString(end_date->getString());
		}
	}
	return kn;
}

char* Knowde::getHash() { return hash; }
char* Knowde::getTitle() { return title->str; }
char* Knowde::getDescr() { return descr->str; }
char* Knowde::getText() { return text->str; }
char* Knowde::getKeywords() { return keywords->str; }
char* Knowde::getDate() { return date->str; }

unsigned long Knowde::getTextSize() { return text->len; }

unsigned long Knowde::getSize() {
	return (title->len + descr->len + text->len + keywords->len);
}

unsigned long Knowde::getSizeRecoursive() {
	unsigned int c; unsigned long sum = getSize();
	for (c=0;c < children.size(); c++)
		sum += children[c]->getSizeRecoursive();
	return sum;
}

Knowde* Knowde::getParent() { return parent; }

int Knowde::countChildren() { return children.size(); }

Knowde* Knowde::getChild(int index) {
	if ((index>=0) && (index < (int) children.size()))
		return children[index];
	else
		return NULL;
}

void Knowde::removeChild(int index) {
	Knowde *knowde = children[index];
	std::vector<Knowde*>::iterator iter;

	if (index>= (int) children.size()) return;
	while (knowde->countChildren())
		knowde->removeChild(0);

	iter = children.begin();
	children.erase(iter+index);
}

void Knowde::setTitle(char *_title) {
	g_string_assign(title,_title);
}

void Knowde::setDescr(char *_descr) {
	g_string_assign(descr,_descr);
}

void Knowde::setText(char *_text) {
	g_string_assign(text,_text);
}

void Knowde::setKeywords(char *_keywords) {
	g_string_assign(keywords,_keywords);
}

Knowde* Knowde::newChild(char* hash) {
	Knowde *child = new Knowde(this, hash);
	children.push_back(child);
	return child;	
}

DatePoint* Knowde::getStartDate() { return start_date; }
DatePoint* Knowde::getEndDate() { return end_date; }

int Knowde::hasDate() { if (start_date) return TRUE; else return FALSE; }
int Knowde::isDateDurative() { if (end_date) return TRUE; else return FALSE; }

void Knowde::enableDate() {
	if (!start_date) start_date = new DatePoint();
}

void Knowde::disableDate() {
	if (end_date) { delete end_date; end_date = NULL; }
	if (start_date) { delete start_date; start_date = NULL; }
}

void Knowde::setDateDurative(int yesno) {
	if (yesno) {
		if (!start_date) return;
		end_date = new DatePoint();
	} else {
		if (!start_date) return;
		if (!end_date) return;
		delete end_date; end_date = NULL;
	}
}

void Knowde::normalizeDate() {
	DatePoint *tmp; char buf[128];
	
	if (!start_date) {
		g_string_assign(date,"");
		return;
	}
	if (!end_date) {
		g_string_assign(date,start_date->getString());
		return;
	}
	// create values and strings
	// start_date->normalize();
	// end_date->normalize();

	// if not sorted, sort it
	if (start_date->getValue() > end_date->getValue()) {
		tmp = start_date;
		start_date = end_date;
		end_date = tmp;
	}

	sprintf(buf,"%s - %s",start_date->getString(),end_date->getString());
	g_string_assign(date,buf);
}

int Knowde::hasTextColor() { return (text_color != NULL); }
int Knowde::hasBackgroundColor() { return (bg_color != NULL); }

void Knowde::disableTextColor() { if (text_color) { free(text_color); text_color = NULL; } }
void Knowde::disableBackgroundColor() { if (bg_color) { free(bg_color); bg_color = NULL; } }

GdkColor* Knowde::getTextColor() { return text_color; }
GdkColor* Knowde::getBackgroundColor() { return bg_color; }

int Knowde::hasTextFont() { return (text_font != NULL); }

void Knowde::disableTextFont() { if (text_font) { g_string_free(text_font, TRUE); text_font = NULL; } }

char* Knowde::getTextFont() { if (!text_font) return NULL; else return text_font->str; }

void Knowde::setTextFont(char *font) { 
	if (!text_font) 
		text_font = g_string_new(""); 
	g_string_assign(text_font,font);
}

void Knowde::setTextColor(GdkColor *color) { 
	if (!text_color) 
		text_color = (GdkColor*) malloc(sizeof(GdkColor));
	if (!text_color) return;
	memcpy(text_color,color,sizeof(GdkColor));
}

void Knowde::setBackgroundColor(GdkColor *color) { 
	if (!bg_color) 
		bg_color = (GdkColor*) malloc(sizeof(GdkColor));
	if (!bg_color) return;
	memcpy(bg_color,color,sizeof(GdkColor));
}
#endif

