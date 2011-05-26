#ifndef __KNOWDE_FILE_CC__
#define __KNOWDE_FILE_CC__

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "KnowdeFile.h"

#define STANDARD_FILENAME "export.knowde"

KnowdeFile::KnowdeFile(char* _filename) {
	data = new Knowde(NULL);
	if (!_filename) {
		filename = g_string_new("");
	} else {
		filename = g_string_new(_filename);
		load();
	}
}

KnowdeFile::~KnowdeFile() {
	delete data;
}

char* KnowdeFile::getFilename() { 
	return filename->str; 
}

void KnowdeFile::setFilename(char *_filename) {
	g_string_assign(filename,_filename);
}

Knowde* KnowdeFile::getData() { return data; }

void KnowdeFile::removeKnowde(Knowde *knowde) {
	removeKnowde(knowde, data);
}

void KnowdeFile::removeKnowde(Knowde *knowde, Knowde *start) {
	int c;

	c = start->countChildren();
	for (c = c-1; c>=0; c--) {
		if (start->getChild(c) == knowde) {
			start->removeChild(c);
			return;
		} else removeKnowde(knowde,start->getChild(c));
	}
}

void KnowdeFile::disableDefaultTextFont() {
	data->disableTextFont();
}

char* KnowdeFile::getDefaultTextFont() {
	return data->getTextFont();
}

void KnowdeFile::setDefaultTextFont(char *font) {
	data->setTextFont(font);
}

int KnowdeFile::hasDefaultTextFont() {
	return data->hasTextFont();
}

void KnowdeFile::saveXMLString(char *text, int len, FILE *file) {
	unsigned long c; char x;
	if (len==0) len = 32767;
	for (c=0 ; (int) c < len && text[c] ; c++) {
		x = text[c];
		switch (x) {
//			case '\n': fputc('|',file); break;
			case '&': fputs("&amp;",file); break;
			case '\'': fputs("&apos;",file); break;
			case '\"': fputs("&quot;",file); break;
			case '<': fputs("&lt;",file); break;
			case '>': fputs("&gt;",file); break;
			default: fputc(x,file);
		}
	}
}

void KnowdeFile::saveKnowde(Knowde *knowde, FILE *file, int indent) {
	char *text; int c, len; char color[64];
	
	for (c=0;c<indent;c++)
		fputc('\t',file);
	fputs("<knowde hash=\"",file);
	fputs(knowde->getHash(),file);
	fputs("\">\n",file);
	
	// title
	for (c=0;c<=indent;c++)
		fputc('\t',file);
	fputs("<title>",file);
	saveXMLString(knowde->getTitle(),0,file);
	fputs("</title>\n",file);
	
	// description
	for (c=0;c<=indent;c++)
		fputc('\t',file);
	fputs("<description>",file);
	saveXMLString(knowde->getDescr(),0,file);
	fputs("</description>\n",file);
	
	// keywords
	for (c=0;c<=indent;c++)
		fputc('\t',file);
	fputs("<keywords>",file);
	saveXMLString(knowde->getKeywords(),0,file);
	fputs("</keywords>\n",file);

	// start date
	if (knowde->getStartDate()) {
		for (c=0;c<=indent;c++)
			fputc('\t',file);
		fputs("<start_date>",file);
		fputs(knowde->getStartDate()->getString(),file);
		fputs("</start_date>\n",file);
	}

	// end date
	if (knowde->getEndDate()) {
		for (c=0;c<=indent;c++)
			fputc('\t',file);
		fputs("<end_date>",file);
		fputs(knowde->getEndDate()->getString(),file);
		fputs("</end_date>\n",file);
	}

	// text font
	if (knowde->hasTextFont()) {
		for (c=0;c<=indent;c++)
			fputc('\t',file);
		fputs("<text_font>",file);
		fputs(knowde->getTextFont(),file);
		fputs("</text_font>\n",file);
	}

	// text color
	if (knowde->hasTextColor()) {
		for (c=0;c<=indent;c++)
			fputc('\t',file);
		fputs("<text_color>",file);
		sprintf(color,"%u %u %u %u",knowde->getTextColor()->pixel,
											knowde->getTextColor()->red,
											knowde->getTextColor()->green,
											knowde->getTextColor()->blue);
		fputs(color,file);
		fputs("</text_color>\n",file);
	}
	
	// background color
	if (knowde->hasBackgroundColor()) {
		for (c=0;c<=indent;c++)
			fputc('\t',file);
		fputs("<background_color>",file);
		sprintf(color,"%u %u %u %u",knowde->getBackgroundColor()->pixel,
											knowde->getBackgroundColor()->red,
											knowde->getBackgroundColor()->green,
											knowde->getBackgroundColor()->blue);
		fputs(color,file);
		fputs("</background_color>\n",file);
	}
	
	// text
	for (c=0;c<=indent;c++)
		fputc('\t',file);
	fputs("<text>",file);
	text = knowde->getText();
	len = knowde->getTextSize();
	saveXMLString(text,len,file);
	fputs("</text>\n",file);

	len = knowde->countChildren();
	for (c=0;c<len;c++)
		saveKnowde(knowde->getChild(c),file,indent+1);
	
	for (c=0;c<indent;c++)
		fputc('\t',file);
	fputs("</knowde>\n",file);
}

void KnowdeFile::save() {
	FILE *file; int children, c;

	if (filename->str[0])
		file = fopen(filename->str,"w+");
	else
		file = fopen(STANDARD_FILENAME,"w+");
	if (!file) { printf("Could not save file!\n"); return; }
	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n",file);
	fputs("<knowdefile>\n",file);
	if (hasDefaultTextFont()) {
		fputs("\t<text_font>",file);
		fputs(getDefaultTextFont(),file);
		fputs("</text_font>\n",file);
	}
	
	children = data->countChildren();
	for (c = 0; c<children; c++)
		saveKnowde(data->getChild(c),file,1);
	fputs("</knowdefile>\n",file);
	fclose(file);
}

void KnowdeFile::saveAs(char *_filename) {
	setFilename(_filename);
	save();
}

void KnowdeFile::loadXMLNode(xmlNode *node, Knowde *_parent) {
	xmlNode *cur_node = NULL; Knowde *knowde = NULL; xmlChar *content = NULL;
	xmlAttr *attributes = NULL; GdkColor color;

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			// printf("node name: %s\n",cur_node->name);
			if (!strcmp((char*) cur_node->name,"knowde")) {
				for (attributes = cur_node->properties; attributes; attributes = attributes->next) {
					if (!strcmp((char*)attributes->name,"hash")) {
						content = xmlNodeGetContent(attributes->children);
//						printf("Hash: %s\n",(char*) content);
						break;
					}
				}
				knowde = _parent->newChild((char*) content);
				if (content) {
					xmlFree(content); content = NULL;
				}
			}
			else if (!strcmp((char*) cur_node->name,"title")) {
				content = xmlNodeGetContent(cur_node);
				if (content) { 
					if (_parent)
						_parent->setTitle((char*) content);
					xmlFree(content); content = NULL;
				}
			}
			else if (!strcmp((char*) cur_node->name,"description")) {
				content = xmlNodeGetContent(cur_node);
				if (content) { 
					if (_parent)
						_parent->setDescr((char*) content);
					xmlFree(content); content = NULL;
				}
			}
			else if (!strcmp((char*) cur_node->name,"keywords")) {
				content = xmlNodeGetContent(cur_node);
				if (content) { 
					if (_parent)
						_parent->setKeywords((char*) content);
					xmlFree(content); content = NULL;
				}
			}
			else if (!strcmp((char*) cur_node->name,"text")) {
				content = xmlNodeGetContent(cur_node);
				if (content) { 
					if (_parent)
						_parent->setText((char*) content);
					xmlFree(content); content = NULL;
				}
			}
			else if (!strcmp((char*) cur_node->name,"start_date")) {
				content = xmlNodeGetContent(cur_node);
				if (content) { 
					if (_parent) {
						_parent->enableDate();
						if (_parent->getStartDate()) {
							_parent->getStartDate()->fromString((char*) content);
							_parent->normalizeDate();
						}
					}
					xmlFree(content); content = NULL;
				}
			}
			else if (!strcmp((char*) cur_node->name,"end_date")) {
				content = xmlNodeGetContent(cur_node);
				if (content) {
					if (_parent) {
						_parent->setDateDurative(TRUE);
						if (_parent->getEndDate()) {
							_parent->getEndDate()->fromString((char*) content);
							_parent->normalizeDate();
						}
					}
					xmlFree(content); content = NULL;
				}
			}
			else if (!strcmp((char*) cur_node->name,"text_font")) {
				content = xmlNodeGetContent(cur_node);
				if (content) { 
					if (_parent)
						_parent->setTextFont((char*) content);
					xmlFree(content); content = NULL;
				}
			}
			else if (!strcmp((char*) cur_node->name,"text_color")) {
				content = xmlNodeGetContent(cur_node);
				if (content) { 
					if (_parent) {
						sscanf((char*) content,"%u %u %u %u", &color.pixel,
																(unsigned int *) &color.red,
																(unsigned int *) &color.green,
																(unsigned int *) &color.blue);
						_parent->setTextColor(&color);
					}
					xmlFree(content); content = NULL;
				}
			}
			else if (!strcmp((char*) cur_node->name,"background_color")) {
				content = xmlNodeGetContent(cur_node);
				if (content) { 
					if (_parent) {
						sscanf((char*) content,"%u %u %u %u", &color.pixel,
																(unsigned int *) &color.red,
																(unsigned int *) &color.green,
																(unsigned int *) &color.blue);
						_parent->setBackgroundColor(&color);
					}
					xmlFree(content); content = NULL;
				}
			}
			
		} // end if XML_ELEMENT_NODE
		if (!knowde) knowde = _parent;
		loadXMLNode(cur_node->children, knowde);
	}
}

void KnowdeFile::load() {
	xmlDocPtr doc = NULL;
	xmlParserCtxtPtr ctxt;
	xmlNode *root_node = NULL;
	
	if (!filename->str[0])
		return;
	
	ctxt = xmlNewParserCtxt();
	if (ctxt == NULL) {
		fprintf(stderr,"Could not allocate parser context.\n");
		xmlFreeParserCtxt(ctxt);
		xmlCleanupParser();
		return;
	}
	
	doc = xmlCtxtReadFile(ctxt, filename->str, NULL, XML_PARSE_DTDVALID);
	if (!doc) {
		fprintf(stderr,"Could not parse %s\n",filename->str);
		xmlFreeParserCtxt(ctxt);
		xmlCleanupParser();
		return;
	}

/*	 if (ctxt->valid == 0) {
		fprintf(stderr,"Failed to validate %s\n",filename->str);
		xmlFreeDoc(doc);
		xmlFreeParserCtxt(ctxt);
		xmlCleanupParser();
		return;
	}
*/

	// now we have to parse the document tree
	root_node = xmlDocGetRootElement(doc);
	loadXMLNode(root_node,data);
	
	xmlFreeDoc(doc);
	xmlFreeParserCtxt(ctxt);
	xmlCleanupParser();
}

#endif

