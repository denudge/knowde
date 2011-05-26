#ifndef __MAIN_CC__
#define __MAIN_CC__

#include <gtk/gtk.h>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
	int c;
	FILE *file;
	
	gtk_init(&argc,&argv);

	MainWindow *window = new MainWindow(NULL);

	// load parametered filename
	for (c=1; c<argc; c++) {
		file = fopen(argv[c],"r");
		if (file) {
			fclose(file);
			window->loadFile(argv[c]);
			break;
		}
	}

	gtk_main();
	return 0;
}

#endif

