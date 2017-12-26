/*************************************************************************
Project : C-Refine Precompiler
Module  : Declarations for global Functions, Types and Data
Author  : Lutz Prechelt, Karlsruhe
Date    : 12.06.92  Version 17
Compiler: ANSI C
**************************************************************************/
/*
    Copyright (C) 1988,89,90,91 by Lutz Prechelt, Karlsruhe

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

/*************************************************************************
*********************** C - R e f i n e **********************************
**************************************************************************/

#include "std.h"

#if BSD
#include <strings.h>
#else
#include <string.h>
#endif

/***************** globale Typen und Konstanten **************************/

#define normal_line    1        /* Types for LINE_INFO */
#define empty_line     2    /* empty line */
#define refcall_line   3    /* refinement call */
#define refcallr_line  4    /* dito with "removed semicolon" */
#define refdecl_line   5    /* refinement declaration */
#define leave_line     6    /* leave construct */

#define TAB       9       /* ASCII Tabulator has to be recognized */

#if ms_dos
#define std_refinementsymbol   175   /* IBM: Double arrow to the right */
#else
#define std_refinementsymbol    96   /* ASCII: back quote */
#endif

#if ms_dos
#define STD_MAXLINELENGTH  500
#define STD_MAXLINE        800    /* max. nr of lines per function */
#define STD_MAXREF         100    /* max. nr of refinements per function */
#define STD_S_SIZE       60000    /* wanted size of line buffer in bytes */
#else
#define STD_MAXLINELENGTH  500
#define STD_MAXLINE       2500
#define STD_MAXREF         200
#define STD_S_SIZE      150000
#endif
#define STD_MAXERRORS       20
#define STD_MAXWARNINGS     30

typedef char *charp;   /* Needed: the SUN-OS Compiler is a fool */

typedef struct { int type,      /* see #defines */
                     indent,    /* indent+length = width of line */
                     level,     /* Level at beginning of line */
                     line_no,   /* line number in source file */
                     length;    /* stored length in bytes */
                 charp start;
               } LINE_INFO;

typedef struct { charp name;
                 int  firstentry,     /* pointer into LINE_INFO array */
                      calls,          /* nr of usages of this refinement */
                      semicolons;     /* nr of statements in body */
                 unsigned active:1,   /* we are just in it ? */
                          leave_it:1; /* there is a 'leave' for it ? */
               } REF_INFO;

typedef LINE_INFO  *LINES;

typedef REF_INFO   *REFS;

/************************* External Functions ****************************/

/***** crefine *****/
extern void copy_with_doubled_backslashes A((char *string, char *copy));
/***** cr_getln *****/
extern void init_scanner A(());
extern void get_line A((FILE*, LINE_INFO*, int*));
/***** cr_talk *****/
extern void cout A((int));
extern void error A((charp[], charp,  int));
extern void fatal_error A((charp[], charp, int));
extern void warning A((charp[], charp, int, int));

/************************ globale Daten **********************************/


/* Meldungstexte */
extern charp
 usagestring[],
 versionstring[],
 Tlistline[],
 Tnear[],
 Wempty_function[],
 Wlong_ref[],
 Wref_often_used[],
 Wref_unused[],
 Wref_empty[],
 Wcol0[],
 Wnesting[],
 Wmuch_indent[],
 Wlong_string[],
 Wlong_char[],
 Eopen[],
 Ereadinput[],
 Ewriteoutput[],
 Emissing_r[],
 Elevel0_ref[],
 Elines_in_func[],
 Ebytes_in_func[],
 Eref_not_decl[],
 Erecursive_ref[],
 Eunknown_leave[],
 Eunterminated[],
 Eleave_unpresent[],
 Eref_multi_decl[],
 Ememory[],
 Eeof_brace[],
 Eeof_comment[],
 Eeof_string[],
 Eeof_char[],
 Erefdecl_syntax[],
 Erefdecl_nested[],
 Emany_braces[],
 Erefname_missing[],
 Emany_errors[],
 Emany_warnings[];

#ifdef DATA_HERE
#define extern
#endif
/***** Control *****/
extern bool option_anyway,
            option_comment,
            option_indent,
            option_ibmchars,
            option_list,
            option_cplusplus,
            option_verbose,
            option_small,
            stop_processing;

extern int  refinementsymbol,
            tabsize,
            numbering_level,
            feedback_interval,
            msg_type,
            warning_level,
            maxerrors,
            maxwarnings;

/***** Buffer  *****/
extern charp  b,        /* Puffer fuer eine Zeile */
              s,        /* Naechste freie Position im Zeilenspeicher */
              s_root;   /* Beginn des Zeilenspeichers */
extern unsigned
            maxline,
            maxref,
            b_size,   /* Groesse des Puffers fuer eine Zeile */
            s_size;   /* Gesamtgroesse des Zeilenspeichers */

/***** Status *****/
extern int  errors, warnings,   /* numbers of ...*/
            line_no,            /* current (virtual) line number */
            commanded_line_no;  /* line_no according to last line_cmd */

extern bool error_in_this_function;
extern char name_in[200],     /* filenames: input or virtual input */
            modified_name_in[200],  /* (with doubled backslashes) */
            name_out[200];          /* output */

#ifdef DATA_HERE
#undef extern
#endif
