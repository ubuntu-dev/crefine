#Makefile for C-Refine
#Author: Lutz Prechelt, Karlsruhe, Germany
#Date  : 12.06.92

# Usage:
# Perhaps change the macro CC below if you want to use a different compiler.
# Then call
#   make crefine
# to get a version of crefine that is at first made from 
# the unC-Refine'd sources of C-Refine.
# With this version of C-Refine you can then bootstrap C-Refine by
# 'touch'ing the .cr files and again executing
#   make crefine
# You should normally remove the -n0 option from CRFLAGS so to get only such
# messages from your compiler that point to the .cr file instead of to 
# the .c file
#
# Translation is done in two phases with separate rules:
#   First from .cr to .c if the .cr file is newer,
#   then from .c to .o
# In the makefiles of bigger projects I usually prefer to hold all the
# .c files in some tmp directory, so that they don't clobber your working
# directory and to have a single rule going from .cr to .o
# Be careful and keep a copy of 'crefine' when experimenting
# with changes!
#
# Installation can be performed by
#   make install
# which copies the crefine binary, ccr shell script to the directory given
# by the BINDESTDIR variable and copies the crefine.1 manpage to the 
# directory given by the MANDESTDIR variable. Please check and perhaps
# change these variables first.
# 

#-----constants

CLD=	getargs.o  crefine.o  cr_talk.o  cr_getln.o
CS=     getargs.c crefine.c cr_talk.c cr_getln.c
HS=     getargs.h crefine.h cr_talk.h cr_getln.h std.h cr_texts.h

CR=crefine
CRFLAGS= -ac -s- -T8
CC=cc
CFLAGS=  -I"." -DNDEBUG=1 -Ddebug=0 -Dms_dos=0 -Ddeutsch=0 
LDFLAGS= 
BINDESTDIR=/usr/local/bin
MANDESTDIR=/usr/man/man1


#-----rules

.SUFFIXES: .o .c .cr

.cr.c:
	$(CR) $(CRFLAGS) $*.cr

.c.o:
	$(CC) $(CFLAGS) -c $<

#-----program

crefine:  $(CLD)
	$(CC) $(LDFLAGS) -o crefine $(CLD)

getargs.o: getargs.h  std.h

crefine.o: cr_decl.h getargs.h std.h

cr_talk.o: cr_decl.h  cr_texts.h  std.h

cr_getln.o: cr_decl.h std.h

#-----documentation

crefine.man: crefine.1
	nroff -man crefine.1 | col -b >crefine.man

ccr.man: ccr.1
	nroff -man ccr.1 | col -b >ccr.man

#-----cleanup

clean:
	-rm -f core  *~  *.o
	-strip crefine

completely_clean:  clean
	-rm -f *.c crefine

#-----installation

install:
	-strip crefine
	cp crefine ccr $(BINDESTDIR)
	chmod a=rx $(BINDESTDIR)/crefine $(BINDESTDIR)/ccr
	cp crefine.1 $(MANDESTDIR)
	chmod a=r $(MANDESTDIR)/crefine.1
	@echo "C-Refine installed successfully"

#-----packaging for delivery

TARFILE=crefine.3.0.tar
ZOOFILE=crefine.zoo
FILES1=README MANIFEST INSTALL COPYING TO_DO Makefile ccr \
 std.h cr_decl.h cr_texts.h getargs.h patchlevel.h
FILES_man=crefine.man crefine.1 ccr.man ccr.1
FILES_cr=$(FILES_cr_1) $(FILES_cr_2)
FILES_cr_1=crefine.cr
FILES_cr_2=cr_getln.cr cr_talk.cr erato.cr getargs.cr
FILES_c=$(FILES_c_1) $(FILES_c_2)
FILES_c_1=crefine.c
FILES_c_2=cr_getln.c cr_talk.c erato.c getargs.c
ALL_FILES=$(FILES1) $(FILES_man) $(FILES_cr) $(FILES_c)

release: shar $(TARFILE) $(ZOOFILE)

shar: $(ALL_FILES) MANIFEST
	shar $(FILES1) >crefine.1.shar
	shar $(FILES_man) >crefine.2.shar
	shar $(FILES_cr_1) >crefine.3.shar
	shar $(FILES_cr_2) >crefine.4.shar
	shar $(FILES_c_1) >crefine.5.shar
	shar $(FILES_c_2) >crefine.6.shar

tar: $(TARFILE)

zoo: $(ZOOFILE)

MANIFEST: Makefile
	/bin/ls $(ALL_FILES) >MANIFEST

c_files.stamp: $(FILES_c)
	crefine -qac -s- -n0 -T8 $(FILES_cr)
	touch c_files.stamp

$(TARFILE): $(FILES1) $(FILES_man) $(FILES_cr) c_files.stamp
	tar cvf $(TARFILE) $(ALL_FILES)
	yes "y" | compress -v $(TARFILE)

$(ZOOFILE): $(FILES1) $(FILES_man) $(FILES_cr) c_files.stamp
# actually, the C files should have LF -> CR LF filtering before they
# are packed into a zoo file.
