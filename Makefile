#Makefile for C-Refine
#Author: Lutz Prechelt, Karlsruhe, Germany
#Date  : 11.06.91

# Usage:
# Perhaps change the macro CC below if you want to use a different compiler
# and in CFLAGS set 'ansi=1' if it is ANSI-C.
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
# .c files in some temp directory, so that they don't clobber your working
# directory and to have a single rule going from .cr to .o
# Be careful to keep a copy of 'crefine' when experimenting
# with changes!
#
CLD=	getargs.o  crefine.o  cr_talk.o  cr_getln.o
CS=     getargs.c crefine.c cr_talk.c cr_getln.c
HS=     getargs.h crefine.h cr_talk.h cr_getln.h std.h cr_texts.h

CR=crefine
CRFLAGS= -ac -s- -T8
CC=cc
CFLAGS=  -I"." -DNDEBUG=1 -Ddebug=0 -Dunix=1 -Dms_dos=0 -Dansi=0 -Ddeutsch=0
LDFLAGS= 

%.c: %.cr
	$(CR) $(CRFLAGS) $*.cr

%.o: %.c
	$(CC) $(CFLAGS) -c $<

crefine: a.out
	mv a.out crefine

a.out:  $(CLD)
	$(CC) $(LDFLAGS) $(CLD)

getargs.o: getargs.h  std.h

crefine.o: cr_decl.h getargs.h std.h

cr_talk.o: cr_decl.h  cr_texts.h  std.h

cr_getln.o: cr_decl.h std.h

clean:
	-rm core  a.out  *~  crefine.c  cr_getln.c  cr_talk.c  *.o



