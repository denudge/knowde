HEADERS=`pkg-config --cflags gtk+-2.0` `xml2-config --cflags`
LIBS=`pkg-config --libs gtk+-2.0` `xml2-config --libs`
OBJS=obj/main.o obj/Knowde.o obj/KnowdeFile.o obj/MainWindow.o obj/ListView.o obj/TreeView.o obj/Menu.o obj/Editor.o obj/Toolbar.o obj/Reader.o obj/Statusbar.o obj/Searcher.o obj/DatePoint.o obj/Dater.o obj/Abouter.o obj/TimeView.o obj/ExportHTML.o obj/Layouter.o
CC=g++ -march=i686 -mcpu=i686 -O4 -Wall

knowde: $(OBJS)
	$(CC) $(LIBPATH) -o bin/knowde $(OBJS) $(LIBS)
	strip -s bin/knowde

obj/main.o: src/main.cc
	$(CC) -c $(HEADERS) src/main.cc -o obj/main.o

obj/KnowdeFile.o: src/KnowdeFile.cc src/KnowdeFile.h src/Knowde.h
	$(CC) -c $(HEADERS) src/KnowdeFile.cc -o obj/KnowdeFile.o

obj/Knowde.o: src/Knowde.cc src/Knowde.h src/DatePoint.h
	$(CC) -c $(HEADERS) src/Knowde.cc -o obj/Knowde.o

obj/MainWindow.o: src/MainWindow.cc src/MainWindow.h src/TreeView.h src/Menu.h src/Controller.h src/config.h src/Widget.h src/Editor.h src/KnowdeFile.h src/Knowde.h src/Toolbar.h src/Statusbar.h src/Searcher.h
	$(CC) -c $(HEADERS) src/MainWindow.cc -o obj/MainWindow.o

obj/ListView.o: src/ListView.cc src/ListView.h src/TreeView.h
	$(CC) -c $(HEADERS) src/ListView.cc -o obj/ListView.o

obj/TreeView.o: src/TreeView.cc src/TreeView.h src/Widget.h src/Controller.h src/TreeXpm.h
	$(CC) -c $(HEADERS) src/TreeView.cc -o obj/TreeView.o

obj/Menu.o: src/Menu.cc src/Menu.h src/Controller.h src/Widget.h
	$(CC) -c $(HEADERS) src/Menu.cc -o obj/Menu.o

obj/Editor.o: src/Editor.cc src/Editor.h src/Widget.h src/Knowde.h src/Dater.h src/Layouter.h
	$(CC) -c $(HEADERS) src/Editor.cc -o obj/Editor.o

obj/Toolbar.o: src/Toolbar.cc src/Toolbar.h src/Widget.h src/Controller.h src/ToolbarXpm.h
	$(CC) -c $(HEADERS) src/Toolbar.cc -o obj/Toolbar.o

obj/Reader.o: src/Reader.cc src/Reader.h src/Widget.h src/Controller.h src/Help.h
	$(CC) -c $(HEADERS) src/Reader.cc -o obj/Reader.o

obj/Statusbar.o: src/Statusbar.cc src/Statusbar.h src/Widget.h src/Controller.h
	$(CC) -c $(HEADERS) src/Statusbar.cc -o obj/Statusbar.o

obj/Searcher.o: src/Searcher.cc src/Searcher.h src/Widget.h src/Controller.h
	$(CC) -c $(HEADERS) src/Searcher.cc -o obj/Searcher.o

obj/DatePoint.o: src/DatePoint.cc src/DatePoint.h
	$(CC) -c $(HEADERS) src/DatePoint.cc -o obj/DatePoint.o

obj/Dater.o: src/Dater.cc src/Dater.h src/Widget.h src/DatePoint.h
	$(CC) -c $(HEADERS) src/Dater.cc -o obj/Dater.o

obj/Abouter.o: src/Abouter.cc src/Abouter.h src/Widget.h
	$(CC) -c $(HEADERS) src/Abouter.cc -o obj/Abouter.o

obj/TimeView.o: src/TimeView.cc src/TimeView.h src/ListView.h
	$(CC) -c $(HEADERS) src/TimeView.cc -o obj/TimeView.o

obj/ExportHTML.o: src/ExportHTML.cc src/ExportHTML.h src/Export.h
	$(CC) -c $(HEADERS) src/ExportHTML.cc -o obj/ExportHTML.o

obj/Layouter.o: src/Layouter.cc src/Layouter.h src/Layouter.h
	$(CC) -c $(HEADERS) src/Layouter.cc -o obj/Layouter.o

install:
	install -d -o root -g root -m 0755 /usr/local/bin
	install -o root -g root -m 0755 bin/knowde /usr/local/bin

clean:
	rm -rf obj/*.o *~ src/*~ bin/knowde

pot:
	xgettext -j --omit-header --keyword=_ --sort-output -o po/messages.po src/*.cc

