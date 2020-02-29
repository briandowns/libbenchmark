CC      = cc
CFLAGS  = -Wall -fPIC -O3
LDFLAGS = 

NAME    = libbenchmark
VERSION = 01

TSTDIR := ./tests
INCDIR := /usr/local/include
LIBDIR := /usr/local/lib

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
$(NAME).$(VERSION).so:
	$(CC) -c $(CFLAGS) -shared -o $(NAME).$(VERSION).so benchmark.c  $(LDFLAGS)
endif
ifeq ($(UNAME_S),Darwin)
$(NAME).$(VERSION).dylib:
	$(CC) -c $(CFLAGS) -dynamiclib -o $(NAME).$(VERSION).dylib benchmark.c  $(LDFLAGS)
endif

.PHONY: install
install: 
	cp benchmark.h $(INCDIR)
ifeq ($(UNAME_S),Linux)
	cp $(NAME).$(VERSION).so $(LIBDIR)
endif
ifeq ($(UNAME_S),Darwin)
	cp $(NAME).$(VERSION).dylib $(LIBDIR)
	ln -s $(LIBDIR)/$(NAME).$(VERSION).dylib $(LIBDIR)/$(NAME).dylib
endif

uninstall:
	rm -f $(INCDIR)/benchmark.h
ifeq ($(UNAME_S),Linux)
	rm -f $(LIBDIR)/$(NAME).$(VERSION).so
	rm -f $(LIBDIR)/$(VERSION).so
endif
ifeq ($(UNAME_S),Darwin)
	rm -f $(LIBDIR)/$(NAME).$(VERSION).dylib
	rm -f $(LIBDIR)/$(NAME).dylib
endif

.PHONY:
test: clean
	$(CC) -o $(TSTDIR)/$(TSTDIR) $(TSTDIR)/$(TSTDIR).c benchmark.c $(TSTDIR)/unity/unity.c $(CFLAGS)
	$(TSTDIR)/$(TSTDIR)
	rm -f $(TSTDIR)/$(TSTDIR)

.PHONY: clean
clean:
	rm -f $(TSTDIR)/$(TSTDIR)
