CC = cc

CFLAGS  = -Wall -fPIC -O3
LDFLAGS = 

NAME    = libbenchmark
VERSION = 01

TSTDIR := ./tests
INCDIR := /usr/local/include
LIBDIR := /usr/local/lib

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
$(NAME).dylib: clean
	$(CC) -c $(CFLAGS) -dynamiclib -o $(NAME).dylib benchmark.c  $(LDFLAGS)
else
$(NAME).so: clean
	$(CC) -shared $(CFLAGS) -o $(NAME).so benchmark.c  $(LDFLAGS)
endif

.PHONY: install
install: 
	cp benchmark.h $(INCDIR)
ifeq ($(UNAME_S),Darwin)
	cp $(NAME).dylib $(LIBDIR)
else
	cp $(NAME).so $(LIBDIR)
endif

uninstall:
	rm -f $(INCDIR)/benchmark.h
ifeq ($(UNAME_S),Darwin)
	rm -f $(LIBDIR)/$(NAME).dylib
else
	rm -f $(LIBDIR)/$(NAME).so
endif

.PHONY:
test: clean
	$(CC) -o $(TSTDIR)/$(TSTDIR) $(TSTDIR)/$(TSTDIR).c benchmark.c $(TSTDIR)/unity/unity.c $(CFLAGS)
	$(TSTDIR)/$(TSTDIR)
	rm -f $(TSTDIR)/$(TSTDIR)

.PHONY: clean
clean:
	rm -f $(TSTDIR)/$(TSTDIR)
