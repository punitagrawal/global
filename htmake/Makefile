PROG = htconfig htmake

DESTDIR = /usr
BINDIR = $(DESTDIR)/bin
MANDIR = $(DESTDIR)/man

all:
install:
	@for file in $(PROG); do\
		cp $$file.pl $(BINDIR)/$$file;\
		chmod 755 $(BINDIR)/$$file;\
		cp $$file.1 $(MANDIR)/man1;\
		chmod 644 $(MANDIR)/man1/$$file.1;\
	done
clean:

