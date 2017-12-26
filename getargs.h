/*************************************************************************
Module  : getargs -- command line option processor
Author  : Lutz Prechelt, Karlsruhe
Date    : 11.06.91  Version 2
Compiler: should be portable
          tried: MS-DOS 3.2, Microsoft C 5.0 (ANSI-C)
          SUN-OS 4.03, 4.1 (K&R C)
**************************************************************************/
/*
    Copyright (C) 1988,91 by Lutz Prechelt, Karlsruhe

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/************************************************************************
*********************** g e t a r g s ***********************************
*************************************************************************/

#define INTEGER   0
#define BOOLEAN   1
#define CHARACTER 2
#define STRING    3

typedef struct {
	char arg;
	char type;
	int  *variable;
	char *errmsg;
	} ARG;

#define ARGTABSIZE(a) (sizeof (a) / sizeof (ARG))

extern int  getargs A((int *argc, char **argv, ARG argtab[], int argtabsize));
extern void print_usage A((char *progname, char *usage,
                           ARG tabp[], int argtabsize));

#if 0

Example or template:

/* Definition global (Initialization of Array) ! */

static ARG argtab[] = {
  {'x', TYPE,   &var_x,      "function of option x" },
  .
  .
};

/* Call : */

  if (getargs (&argc, argv, argtab, ARGTABSIZE (argtab)) || argc != 3)
     print_usage (argv[0], "[options] inputfile outputfile [options]",
                  argtab, ARGTABSIZE (argtab));

#endif

