#include "ExportHTML.h"

const char *HEADER[] = {
	"<html>\n",
	"\t<head>\n",
	"\t\t<meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\">\n",
	"\t\t<meta name=\"title\" content=\"Exported Knowde\">\n",
	"\t\t<style type=\"text/css\">\n",
	"\t\t\t<!--\n",
	"\t\t\t.box { font-size:small;font-family:Helvetica;background-color:#AAAAFF;margin:3px;padding:3px;border-width:2px;border-style:solid;border-color:#6666FF; }\n",
	"\t\t\t-->\n",
	"\t\t</style>\n",
	"\t<head>\n",
	"\t<body bgcolor=\"#FFFFFF\">\n",
	"\t\t<table width=\"600\" cellspacing=\"0\" cellpadding=\"0\" border=\"0\"><tr><td>\n",
	0
};

const char *FOOTER[] = {
	"\t\t</td></tr></table>\n",
	"\t</body>\n",
	"</html>\n",
	0
};

int ExportHTML::exportKnowde(Knowde *knowde, char *target_file) {
	FILE *file; int line;
	
	if ((!target_file) || (!knowde)) return -1;
	file = fopen(target_file,"w+");
	if (!file) { printf("Could not save file!\n"); return -1; }
	for (line=0;HEADER[line];line++)
		fputs(HEADER[line],file);
	
	// Title
	fputs("\t\t\t<h1 align=\"center\">",file);
	saveXMLString(knowde->getTitle(),0,file);
	fputs("</h1>\n",file);
	
	// Description
	fputs("\t\t\t<p class=\"box\">",file);
	fputs("<b>Description:</b> ",file);
	saveXMLString(knowde->getDescr(),0,file);
	fputs("</p>\n\n",file);

	// Date
	if (knowde->getStartDate()) {
		fputs("\t\t\t<p class=\"box\"><b>Date:</b> ",file);
		fputs(knowde->getStartDate()->getString(),file);
		if (knowde->getEndDate()) {
			fputs(" - ",file);
			fputs(knowde->getEndDate()->getString(),file);
		}
		fputs("\t\t\t</p>\n\n",file);
	}

	// Keywords
	fputs("\t\t\t<p class=\"box\">",file);
	fputs("<b>Keywords:</b> ",file);
	saveXMLString(knowde->getKeywords(),0,file);
	fputs("</p>\n\n",file);
	
	// Text
	fputs("\t\t\t<p class=\"box\">",file);
	saveXMLString(knowde->getText(),0,file);
	fputs("\t\t\t</p>\n\n",file);
	
	for (line=0;FOOTER[line];line++)
		fputs(FOOTER[line],file);
	fclose(file);
	return 0;
}

int ExportHTML::exportFile(KnowdeFile *file, char *target_file) {
	if (!target_file) return -1;
	return 0;
};

void ExportHTML::saveXMLString(char *text, int len, FILE *file) {
   unsigned long c; char x;
   if (len==0) len = 32767;
   for (c=0 ; (int) c < len && text[c] ; c++) {
      x = text[c];
      switch (x) {
         case '\t': fputs(" &nbsp; &nbsp; &nbsp; \n",file); break;
         case '\n': fputs("<br>\n",file); break;
         case '&': fputs("&amp;",file); break;
         case '\'': fputs("&apos;",file); break;
         case '\"': fputs("&quot;",file); break;
         case '<': fputs("&lt;",file); break;
         case '>': fputs("&gt;",file); break;
         default: fputc(x,file);
      }
   }
}

