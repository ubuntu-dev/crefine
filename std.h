/*************************************************************************
Module  : Header file: mothers little helpers --- bool, assert, prototype args 
Author  : Lutz Prechelt, Karlsruhe
Date    : 11.06.91
Compiler: ANSI C, K&R C
**************************************************************************
/*
    Copyright (C) 1988 by Lutz Prechelt, Karlsruhe

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

#ifndef stdh_
#define stdh_

#if ansi
#define A(a) a
#else
#define A(a) ()
#endif

typedef int  bool;   /* or 'enum' but some debuggers don't like that */
#define true    1
#define false   0

#ifndef NDEBUG

#define _assert_str "\nOOPS !!  Assertion failed: file \"%s\", line %d\
  (CR to continue)"
static char _assert_string[] = _assert_str;
#define _avoid_str "\nOOPS !!  %d happened in file \"%s\", line %d\
  (CR to continue)"
static char _avoid_string[]  = _avoid_str;

#define assert(e)  \
	 (!(e)  ? ( \
	             fprintf(stderr, _assert_string,  __FILE__, __LINE__), \
	             fgetc (stdin) \
             ) \
                : 1 \
	)

#define avoid(e,u)  \
        ((e)!=(u) ? ( \
                    fprintf(stderr, _avoid_string, (e), __FILE__, __LINE__), \
                    fgetc (stdin) \
                    ) \
                  : 1 \
        )

#else

#define assert(e)    1
#define avoid(e,u)   1

#endif /* NDEBUG */


#endif
