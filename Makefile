SHELL = /bin/sh
CC    = g++
FLAGS = -O2 -Wno-deprecated-declarations -Wno-unused-result
 
PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin
INCLUDEDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
 
all: 
	@mkdir -p bin
	@mkdir -p lib
	$(CC) src/tracerSum.cpp $(FLAGS) -o bin/tracerSum
	$(CC) src/graphDiff.cpp $(FLAGS) -o bin/graphDiff
	$(CC) src/graphMerge.cpp $(FLAGS) -o bin/graphMerge
	$(CC) src/graphMergeMultiple.cpp $(FLAGS) -o bin/graphMergeMultiple
	$(CC) -fPIC -shared src/Stacktracer.cpp -Wl,-soname,libstacktracer $(FLAGS) -o lib/libstacktracer.so

install:
	install -D bin/tracerSum $(BINDIR)
	install -D bin/graphDiff $(BINDIR)
	install -D bin/graphMerge $(BINDIR)
	install -D bin/graphMergeMultiple $(BINDIR)
	install -D lib/libstacktracer.so $(LIBDIR)
	install -D src/Stacktracer.h $(INCLUDEDIR)
 
uninstall:
	-rm -f $(BINDIR)/tracerSum
	-rm -f $(BINDIR)/graphDiff
	-rm -f $(BINDIR)/graphMerge
	-rm -f $(BINDIR)/graphMergeMultiple
	-rm -f $(LIBDIR)/libstacktracer.so
	-rm -f $(INCLUDEDIR)/Stacktracer.h
 
clean:
	-rm -f -r bin lib
 
.SECONDEXPANSION:
 
$(foreach OBJ,$(OBJECTS),$(eval $(OBJ)_DEPS = $(shell gcc -MM $(OBJ:.o=.c) | sed s/.*://)))
%.o: %.c $$($$@_DEPS)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -c -o $@ $<
 
# %.o: %.c $(HEADERS) $(COMMON)
#    $(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -c -o $@ $<
 
.PHONY : all profile release \\
	install install-strip uninstall clean distclean

