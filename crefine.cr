/*************************************************************************
Project : C-Refine Precompiler
Module  : main module
Author  : Lutz Prechelt, Karlsruhe
Date    : 12.06.92  Version 17
Compiler: C, C-Refine
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

/************************************************************************
*********************** C - R e f i n e *********************************
*************************************************************************/

#if 0
REMARK:
  This program was originally written using german identifiers and
  comments. I worked through it to change this (at least in most parts)
  but please do not flame me if there are relicts from this state.
  I did not find a sensible replacement for the german word 'Klammer' that
  may stand for any of the characters ( ) [ ] { }  so sometimes I still
  use the german one.

Variants:
#define deutsch   fuer deutsche Meldungen statt englische
#define ms_dos    for  MS-DOS "Operating System"
#define __STDC__  for  ANSI-C Compiler (as opposed to K&R-C)

For version changes see refinement startup message and file cr_texts.h

============================== History: ====================================


Version 1.0  (Alpha)    (0, 1)

1987
  procedural refinements only
  not comfortable, few error messages


Version 2.0  (Alpha)    (2, 3)

Feb. 88
  value-returning refinements,
  improved error handling,
  Options comment, feedback, list, numbering, refinementsymbol, small


Version 2.1  (Beta)     (4, 5)

09.04.88  ( --> Martin, Ihno, Volker, Beat)
  improved error handling
  triple error messages (german, english, german nonsense),
  Context description in error messages,
  Options anyway, feedback, ibm chars, message type, tabsize, warning level
  Expires 31.07.88


Version 2.2             (6, 7, 8)

08.07.88   ( --> Martin)
  corr: Changes blanks in namen to underscrores again (like in Version 1.0)
        (otherwise error in goto occur)
  corr: Removed semicolon after value-returning refinements kept
        (via introduction of refcallr)
  improved error handling: Warning for "often used" only for
  "big" refinements.
03.08.88
  corr: When inserting file names in "#line"-commands, backslashes
        are doubled (backslash is Escape symbol in C string denoters)
13.09.88
  corr: empty refinements will not give syntax errors anymore.


Version 2.3             (9, 10, 11, 12, 13)

29.09.88 ( --> Martin, Ihno)
  corr: refinements with just 1 semicolon are enclosed in braces also
        (to avoid "else"-conflict)
  C++ Mode (i.e. // Kommentare) as option p
17.03.89
  "std.h" introduced,  #ifdefs for environment switches introduced.
  Look-through, further "refinementized", several small corrections
  ported to SUN-OS (Berkeley-Unix)
  Size restrictions released (500 -> 800 lines, 50 -> 100 Refinements)
18.03.89
  dito, expires 31.07.89
11.09.89
  ported to PCS maschine under Munix 5.3 (System V)
  further #ifdefs for deutsch, expire
  The switches are given in the Makefile
11.09.89
  dito, expires 31.10.89


Version 2.4             (14, 15, 16)

27.08.90 ( --> Uni)
  Line numbering switchable in 4 levels.
  Quiet-Option
  Buffersizes selectable via option.
  New name handling for input/output files
  
23.01.91 ( --> Uni, Usenet announcement, iraun1)
  Names and Comments changed to english (was german before)
  error in level switching of line numbering fixed.
  
10.06.91 (comp.sources.misc)
  History translated to english
  some small corrections
  corr: REF_INFO storage has to be 0-initialized, 
        so use calloc instead of malloc
  Eliminated duplication of function declarations for ansi and non-ansi
  by introduction of the A(a) macro in std.h  


Version 3.0             (17)

15.11.91
  Getargs deeply changed: disallow mixing of options and normal args but
  allow values for options to be in separate argument.
  Handle multiple input file names.
  Introduced -o option for explicit output filename, all other are built
  by deleting a trailing r from input filename.
  Handle "-" for stdin and stdout in filenames.
  React on #line commands in input file.  !!!INCOMPLETE!!!
  Changed #include commands in sourcess for better portability
  Replaced 'ansi' #define with '__STDC__'
12.06.92 (comp.sources.reviewed)
  replaced -q (quiet) flag with a -v (verbose) flag with inverse meaning.
  added humorous messages for english, changed -m option appropriately.

=============================================================================
#endif

#include <stdio.h>

#if __STDC__
#include <stdlib.h>
#else
extern char* malloc();  /* some systems don't have malloc.h or the like */
extern char* calloc();
#endif

#include <time.h>

#define DATA_HERE     /* Here the data declarations are NOT extern */
#include "cr_decl.h"  /* global Functions, Types and Data */
#include "getargs.h"


/******************* lokal Functions ************************************/

extern int  main A((int argc,  charp argv[]));
static int  crefine A(());
static void process_c_refine A((FILE *in, FILE *out));
static void process_line A((FILE *out, int semicolons));
static void insert_refinements A((FILE *out));
static void insert_refinement A((FILE *out, LINE_INFO *l, int startindent,
                               int recursive_level));
static void line_cmd A((FILE* out, int nr, int indent, int min_level));
static void put_indent A((FILE *out, int how_much));
static void put_line A((FILE *out, LINE_INFO *line, int additional_indent,
                      int min_linenumbering_level));
static int  refinement_nr A((char *refinementname));
static void reset_l_r_s A(());

/*********************** Lokal Data ************************************/

/***** line info, refinement info *****/
static LINES l;           /* line info */
static int   l_pos;

static REFS  r;           /* refinement info */
static int   r_pos;

/***** Status *****/
static int   old_level;          /* level at line before */

/***** Sonstiges *****/
static char  blanks[] =  /* indents are made from this. */
  "                                                                       ";
/* "blanks" is taken as  72 Blanks long (see put_indent) */

static char *outputfilename = "";
static int   option_msg;

#define refdecl_line_nr(i)  /* line number of refinement head of r[i] */\
                          (l [r[i].firstentry].line_no)

/******************************** crefine *********************************/

static ARG argtab[] = {
#if  deutsch
  {'a', BOOLEAN, &option_anyway,
   "alle: Ausgabedatei trotz Fehlern nicht loeschen" },
  {'c', BOOLEAN, &option_comment,  "Refinementnamen-Kommentare in Ausgabe" },
  {'e', BOOLEAN, &option_indent,   "#line Kommandos einruecken" },
#if ms_dos
  {'I', BOOLEAN, &option_ibmchars, 
   "erlaube IBM internationale Zeichen fuer Namen" },
#endif
  {'k', BOOLEAN, &option_comment,  "Refinementnamen-Kommentare in Ausgabe" },
  {'l', BOOLEAN, &option_list,     "liste alle Refinementnamen" },
  {'m', CHARACTER, &option_msg,    
   "Meldungen in: d=deutsch, e=english, Grossbuchstabe -> humorig" },
  {'n', INTEGER, &numbering_level,
   "Stufe der Numerierung mit #line Kommandos" },
  {'o', STRING,  &outputfilename,  "setze Name der Ausgabedatei" },
  {'p', BOOLEAN, &option_cplusplus,"C++ Modus" },
  {'v', BOOLEAN, &option_verbose,    "Versionsmeldung ausgeben" },
  {'r', CHARACTER, &refinementsymbol, "Refinementsymbol (als Zeichen)" },
  {'R', INTEGER, &refinementsymbol, "Refinementsymbol (als Zahl)" },
  {'s', BOOLEAN, &option_small,    "strippen: Kompaktifizierte Ausgabe" },
  {'w', INTEGER, &warning_level,   "Warnstufe (0 - 3)" }
  {'A', INTEGER, &feedback_interval, "Intervall der Fortschrittsanzeige" },
  {'F', INTEGER, &maxerrors,       "Max. Anzahl Fehler" },
  {'L', INTEGER, &(int*)b_size,    "Max. Zeilenlaenge in Byte" },
  {'N', INTEGER, &maxref,          "Max. Refinements je Funktion" },
  {'P', INTEGER, &(int*)s_size,    "Codepuffer in Bytes" },
  {'T', INTEGER, &tabsize,         "Tabulatorgroesse" },
  {'W', INTEGER, &maxwarnings,     "Max. Anzahl Warnungen" },
  {'Z', INTEGER, &maxline,         "Max. Anzahl Zeilen je Funktion" },
#else
  {'a', BOOLEAN, &option_anyway,   "anyway: don't delete output on errors" },
  {'c', BOOLEAN, &option_comment,  "comment refinement names in output" },
  {'i', BOOLEAN, &option_indent,   "indent the #line commands" },
#if ms_dos
  {'I', BOOLEAN, &option_ibmchars,
   "enable IBM International Charset for names" },
#endif
  {'l', BOOLEAN, &option_list,     "list all refinement names" },
  {'m', CHARACTER, &option_msg,  
   "Messages in: g=german, e=english, G,E=humorous" },
  {'n', INTEGER, &numbering_level,
   "level of numbering with #line commands" },
  {'o', STRING,  (int*)&outputfilename,  "set output filename" },
  {'p', BOOLEAN, &option_cplusplus,"C++ mode" },
  {'v', BOOLEAN, &option_verbose,    "verbose mode (startup message)" },
  {'r', CHARACTER, &refinementsymbol, "refinementsymbol (character form)" },
  {'R', INTEGER, &refinementsymbol, "refinementsymbol (decimal form)" },
  {'s', BOOLEAN, &option_small,    "small compactified output" },
  {'w', INTEGER, &warning_level,   "warning level (0 - 3)" },
  {'B', INTEGER, (int*)&s_size,    "code buffer in bytes" },
  {'E', INTEGER, &maxerrors,       "max errors" },
  {'L', INTEGER, (int*)&maxline,   "max lines per function" },
  {'M', INTEGER, (int*)&b_size,    "max length of a line" },
  {'N', INTEGER, (int*)&maxref,    "max refinements per function" },
  {'P', INTEGER, &feedback_interval, "interval of progress display" },
  {'T', INTEGER, &tabsize,         "tab size" },
  {'W', INTEGER, &maxwarnings,     "max warnings" },
#endif
};


extern int main (argc, argv)
  int   argc;
  charp argv[];
{
  /* Analyses options and generates pairs of input/output filenames.
     Then calls  crefine with each of these pairs.
     Returns the number of errors that have been found totally.
  */
  int  wrong_options, total_errors = 0;
  int  i;
  FILE *in, *out;
  bool explicit_outputfilename_given;
  `analysis of options;
  if (wrong_options || argc <= 1) {
     print_usage (argv[0], usagestring[msg_type],
                  argtab, ARGTABSIZE (argtab));
    `startup message;
     exit (100);
  }
  if (option_verbose)
    `startup message;
  `reserve memory and complain if necessary;
  explicit_outputfilename_given = (outputfilename[0] != 0);
  for (i = 1; i < argc; i++)
    `generate filename pair and call crefine;
  return (total_errors);

`analysis of options:
  option_anyway = option_indent = option_comment
                = option_list = option_ibmchars = option_cplusplus
                = false;
  option_small = true;
  numbering_level = 3;
#if deutsch
  option_msg  = 'd';   /* deutsche Meldungen als Standard */
#else
  option_msg  = 'e';   /* english warnings and errors as default */
#endif
  refinementsymbol = std_refinementsymbol;
  outputfilename   = "";
  tabsize          = 1;
  warning_level    = 3;
  maxline          = STD_MAXLINE;
  maxref           = STD_MAXREF;
  b_size           = STD_MAXLINELENGTH;
  s_size           = STD_S_SIZE;
  feedback_interval= 0;  /* feedback off */
  maxerrors        = STD_MAXERRORS;
  maxwarnings      = STD_MAXWARNINGS;
  wrong_options = getargs (&argc, &argv, argtab, ARGTABSIZE (argtab));
  if (option_small) {
     tabsize = 1;
  }
  switch (option_msg) {
    case 'd':
    case 'g':  msg_type = 0;  break;
    case 'e':  msg_type = 1;  break;
    case 'D':
    case 'G':  msg_type = 2;  break;
    case 'E':  msg_type = 3;  break;
  }

`startup message:
  fprintf (stderr, 
           "C-Refine Precompiler   %s\n", versionstring[msg_type]);
  fprintf (stderr, 
           "Copyright (C) 1988-1992  Lutz Prechelt, Karlsruhe\n");

`reserve memory and complain if necessary:
  b      = malloc (b_size);
  r      = (REFS)calloc ((maxref+1), sizeof (REF_INFO));
  l      = (LINES)malloc ((maxline+1) * sizeof (LINE_INFO));
  s_root = malloc (s_size);
  if (!(b && s_root && r && l)) {
     fprintf (stderr, Ememory[msg_type]);
     exit (100);
  }

`generate filename pair and call crefine:
  strcpy (name_in, argv[i]);
  copy_with_doubled_backslashes (name_in, modified_name_in);
  if (explicit_outputfilename_given)
     `use explicit filename;
  else if (!strcmp (name_in, "-"))
     `use standard out;
  else
     `use input file name and strip r;

`use explicit filename:
   strcpy (name_out, outputfilename);
   total_errors += crefine ();

`use standard out:
   strcpy (name_out, "-");
   total_errors += crefine ();
     
`use input file name and strip r:
   strcpy (name_out, argv[i]);
   if (`filename ok) {
     name_out[strlen(name_out)-1] = 0; /* remove 'r' from end */
     total_errors += crefine ();
   }
   else {
     fprintf (stderr, Emissing_r[msg_type], name_in);
     total_errors++;
   }

`filename_ok:
   name_out[strlen(name_out)-1] == 'r'

}


static int crefine ()    /* C-REFINE */
{
  /* Opens files name_in and name_out.
     Then calls  process_c_refine and closes files again.
     Returns the number of errors that have been found
  */
  FILE *in, *out;
  `open files and complain if necessary;
  /* here we go: */
  rewind (in);  /* Begin at the beginning, then proceed until you come */
                /* to the end, there stop.   (from: Alice in Wonderland) */
  process_c_refine (in, out);
  if (in != stdin)
    fclose (in);
  if (out != stdout) {
    fclose (out);   /* Schliessen vor unlink noetig ! */
    if (errors && !option_anyway) /* don't produce errorneous outputfiles */
       unlink (name_out);         /* delete file if possible */
  }
  if (errors || warnings)
#if deutsch
     fprintf (stderr, "%d Fehler%s   %d Warnung%s   Ausgabedatei %s\n",
              errors,   errors   != 1 ? "" : "",
              warnings, warnings != 1 ? "en" : "",
              errors && !option_anyway ?
                 "geloescht" : (errors ? "dennoch erhalten" 
                                       : "ist fragwuerdig"));
#else
     fprintf (stderr, "%d error%s   %d warning%s   Output %s\n",
              errors,   errors   != 1 ? "s" : "",
              warnings, warnings != 1 ? "s" : "",
              errors && !option_anyway ?
                 "deleted" : (errors ? "kept anyway" : "is doubtful"));
#endif
  return (errors);

`open files and complain if necessary:
  if (!strcmp (name_in, "-"))
    in = stdin;
  else {
#if ms_dos
  in = fopen (name_in, "rt");  /* read, translated mode */
#else
  in = fopen (name_in, "r");  /* read */
#endif
  }
  if (in == NULL || ferror (in)) {
     fprintf (stderr, Eopen[msg_type], name_in);
     exit (100);
  }
  if (!strcmp (name_out, "-"))
    out = stdout;
  else {
#if ms_dos
  out = fopen (name_out, "wt");  /* write, translated mode */
#else
  out = fopen (name_out, "w");  /* write */
#endif
  }
  if (out == NULL || ferror (out)) {
     fprintf (stderr, Eopen[msg_type], name_out);
     exit (100);
  }

}

/************************ process_c_refine *********************************/

static void process_c_refine (in, out)
  FILE *in, *out;
{
  /* Reads the file 'in' to the end line by line via 'get_line' and
     generates the C source code by inserting the refinement bodies for
     the calls.
     Generates error messages for undeclares refinements and warnings for
     unused or often used ones.
     Uses the variables stop_processing, errors, warnings,
     s, l, r and the option indicators.
  */
  commanded_line_no = line_no = 0;
  errors = warnings = 0;
  stop_processing = error_in_this_function = false;
  reset_l_r_s ();
  init_scanner ();
  if (numbering_level > 0)
    /* we get a linefeed anyway! */
    fprintf (out, "#line 1 \"%s\"", modified_name_in);
  while (!stop_processing)
     `handle next line;
  if (feedback_interval)
     cout (line_no);

`handle next line:
     int semicolons = 0;
     if (feedback_interval && line_no % feedback_interval == 0)
        cout (line_no);
     if (ferror (in))
        fatal_error (Ereadinput, l[l_pos-1].start, line_no);
     if (ferror (out))
        fatal_error (Ewriteoutput, NULL, line_no);
#undef  test_
#ifdef test_
  usleep (5000);
#endif
     get_line (in, l+l_pos, &semicolons);
     process_line (out, semicolons);
}

/************************** process_line ************************************/

static void process_line (out, semicolons)
  FILE *out;
  int   semicolons;
{
  /* Works on the lines up to the current line l[l_pos] in the way that
     it decides whether a function has ended and thus the insertion of
     refinements has to be started.
     On level 0 all lines are copied from in to out immediately.
     After a state change from level 0 to level 1 all lines (along with
     a lineinfo) are kept until the next transition to level 0 and the
     refinement info is being built.
     If necessary, error messages for overflow or refinement errors and
     warnings for not or multiply used refinements are generated.
  */
  if (r_pos > 0)
     r[r_pos - 1].semicolons += semicolons;
  if (old_level == 0)
     `we came from level 0;
  else
     `we were inside a function or so;

`we came from level 0:
   assert (l_pos == 0);   /* nothing can be stored from level 0 */
   if (l[0].level == 0 || stop_processing)
      `remains on level 0 so just copy it;
   else
      `function has begun;

`remains on level 0 so just copy it:
   if (l[0].type != normal_line && l[0].type != empty_line)
      error (Elevel0_ref, l[0].start, line_no);
   put_line (out, &l[0], 0, 1);
   reset_l_r_s ();

`function has begun:
   error_in_this_function = false;
   old_level = l[0].level;    /* neuen Level merken */
   if (l[0].type == refdecl_line && r_pos < maxref) {  /* empty function */
      r[r_pos].name         = l[0].start;
      r[r_pos].firstentry   = 0;
      r[r_pos].active       = false;
      r[r_pos++].semicolons = 0;
      warning (Wempty_function, NULL, line_no - 1, 3);
   }
   l_pos++;                   /* store line */

`we were inside a function or so:
   if (l[l_pos].level == 0 || stop_processing)
      `but now we are outside;
   else
      `and still keep being in;

`but now we are outside:
   insert_refinements (out);
   put_line (out, &l[l_pos-1], 0, 1);  /* last line (Blockklammer) */
   put_line (out, &l[l_pos], 0, 1);    /* the level 0 line */
   error_in_this_function = false;
   reset_l_r_s ();

`and still keep being in:
   if (l[l_pos].type == refdecl_line && r_pos < maxref) {
      r[r_pos].name         = l[l_pos].start;   /* enter Refinement */
      r[r_pos].active       = false;
      r[r_pos].firstentry   = l_pos;
      r[r_pos++].semicolons = 0;
   }
   old_level = l[l_pos].level;/* store new level */
   l_pos++;                   /* store line */
   if (l_pos >= maxline)
      fatal_error (Elines_in_func, l[l_pos].start, line_no);
   if (s - s_root >= s_size - 150)  /* Reserve too small */
      fatal_error (Ebytes_in_func, l[l_pos].start, line_no);
}

/************************ insert_refinements ******************************/

static void insert_refinements (out)
  FILE *out;
{
  /* Replaces the refinement calls with the bodies, after the whole function
     has been read in.
     In the output #line statements are generated, except if option 
     numbering_level is zero. 
     Comments and indentations are thrown away if option_small is true.
     Comments stating the refinement names are inserted in the output if
     option_comment is true.
  */
  int  i, end;
  bool extern_stop = stop_processing;  /* Protect last function against */
  stop_processing  = false;            /* local use of this variable */
  r[r_pos].firstentry = l_pos-1;       /* line of Blockklammer */
  r[r_pos].name       = NULL;
  `generate refinement list if necessary;
  `find last line to insert;
  for (i = 0; i <= end; i++) {  /* lines up to first ref. declaration */
      switch (l[i].type) {
         case refcall_line  :
         case refcallr_line :
                `insert refinement;
                break;
         case leave_line    :
                `whatshallthatbe;
                break;
         case normal_line   :
                `insert normal line;
                break;
         case empty_line    :
                putc ('\n', out);
                commanded_line_no++;
                break;
         case refdecl_line  :
         default       :
                 assert (false);
      }
  }
  `maybe give sermon on refinements;
  stop_processing = stop_processing || extern_stop;  /* Merging-Restore */

`generate refinement list if necessary:
  if (option_list && r_pos > 0) {
     fputc ('\n', stdout);
     for (i = 0; i < r_pos; i++)
         fprintf (stdout, "(%d) %s\n", refdecl_line_nr (i), r[i].name);
  }

`find last line to insert:
  end = r[0].firstentry - 1;
  while (l[end].type == empty_line)   /* suppress trailing empty lines */
     end--;

`insert refinement:
   insert_refinement (out, l+i, l[i].indent, 1);
   if (stop_processing)
      return;

`whatshallthatbe:
  assert (false);

`insert normal line:
   put_line (out, &l[i], 0, 1);

`maybe give sermon on refinements:
  for (i = 0; i < r_pos; i++)
      if (r[i].semicolons > 50)
         warning (Wlong_ref, l[r[i].firstentry].start,
                  refdecl_line_nr (i), 3);
      else if (r[i].calls > 5 && r[i].semicolons > 2)
         warning (Wref_often_used, l[r[i].firstentry].start,
                  refdecl_line_nr (i), 3);
      else if (r[i].calls == 0)
         warning (Wref_unused, l[r[i].firstentry].start,
                  refdecl_line_nr (i), 1);
}

/************************* insert_refinement ******************************/

static void insert_refinement (out, z, startindent, recursive_level)
  FILE *out;
  LINE_INFO *z;
  int  startindent, recursive_level;
{
  /* Looks for the refinement to insert by its name, computes the range
     of lines to work on and does then do the same as insert_refinements
     does.
     If necessary the refinement name is given as a comment before the
     body is inserted.
     The refinement body is enclosed in delimiters:
       if ref.semicolons == 0 in parentheses (on first and last line)
       if ref.semicolons >= 1 in curly braces (on separate lines)
     The refinement calls are counted and maybe messages generated.
     In case of leave-statements the refinement that shall be leave'd is
     marked, so a label can be generated.
     Errors:
       1. Refinement is not declared
       2. recursive call to refinement
       3. leave is impossible because the refinement is not
          present in static call hierarchy
  */
  int i;
  int  nr, ref_startindent, end;
  assert (startindent > 0);
  nr = refinement_nr (`name);  /* search for refinement */
  if (nr == -1) {
     error (Eref_not_decl, `name, z->line_no);
     return;
  }
  else if (r[nr].active)
     `complain for recursive refinement call;
  else {
     r[nr].calls++;           /* register the call */
     r[nr].active   = true;
     r[nr].leave_it = false;
  }
  end = r[nr+1].firstentry - 1;
  while (l[end].type == empty_line)   /* suppress trailing empty lines */
     end--;
  i = r[nr].firstentry + 1;
  if (i > end)
     warning (Wref_empty, l[r[nr].firstentry].start, refdecl_line_nr (nr), 2);
  else
     `insert the refinement;
  r[nr].active = false;
  return;

`complain for recursive refinement call:
   error (Erecursive_ref, `name, z->line_no);
   stop_processing = true;
   return;

`insert the refinement:
  /* for an empty refinement, this is not called at all! */
  `write indentation and opening klammer;
  ref_startindent = l[i].indent;
  for ( ; i <= end; i++) {
      switch (l[i].type) {
         case refcall_line  :
         case refcallr_line :
                `insert refinement;
                break;
         case leave_line    :
                `insert goto statement;
                break;
         case normal_line   :
                `insert normal line;
                break;
         case empty_line    :
                putc ('\n', out);
                commanded_line_no++;
                break;
         case refdecl_line  :
         default       :
                 assert (false);
      }
  }
  if (r[nr].leave_it)
     `generate label;
  `write closing klammer;

`write indentation and opening klammer:
  int sc = r[nr].semicolons;
  if (sc > 0)
     put_indent (out, startindent);
  putc (sc > 0 ? '{' : '(', out);  /* Klammer auf */
  if (option_comment && sc > 0)
     fprintf (out, Tlistline[msg_type], `name, recursive_level);

`insert refinement:
   insert_refinement (out, l+i,
                      startindent + l[i].indent
                                  - ref_startindent,
                      recursive_level+1);

`insert goto statement:
   int leave_nr = refinement_nr (l[i].start);
   if (leave_nr == -1)
      error (Eunknown_leave, l[i].start, l[i].line_no);
   else if (!r[leave_nr].active)
      error (Eleave_unpresent, l[i].start, l[i].line_no);
   else {
      r[leave_nr].leave_it = true;
      put_indent (out, startindent);
      fprintf (out, "goto %s_%d;",
               l[i].start, r[leave_nr].calls);
   }

`insert normal line:
   put_line (out, &l[i], startindent - ref_startindent, `num level);
   
`num level:
  r[nr].semicolons == 0 ? 3 : 2

`generate label:
  fprintf (out, "\n%s_%d: ;", `name, r[nr].calls);
  commanded_line_no++;

`write closing klammer:
  int sc = r[nr].semicolons;
  if (sc > 0) {
     put_indent (out, startindent);
     putc ('}', out);
  }
  else {
     putc (')', out);
     if (z->type == refcallr_line)  /* semicolon has been removed illegaly */
        putc (';', out);
  }

`name:
  z->start
}

/************************* line_cmd **************************************/

static void line_cmd (out, nr, indent, min_level)
  FILE *out;
  int  nr;
  int  indent;
  int  min_level;
{
  /* Writes a "preprocessor #line directive" including file name if
     requested.
     Is suppressed, if min_level is less than numbering_level.
     Using numbering_level, option_indent, commanded_line_no and name_in
  */
  if (numbering_level >= min_level) {
     if (option_indent)
        put_indent (out, indent);
     else
        putc ('\n', out);
     fprintf (out, "#line %d \"%s\"", nr, modified_name_in);
     commanded_line_no = nr-1; /* #line 3  means: next comes line 3 ! */
  }
}

/********************** put_indent **************************************/

static void put_indent (out, how_far)
  FILE *out;
  int how_far;
{
  putc ('\n', out);      /* begin newline */
  commanded_line_no++;
  if (!option_small)
     fwrite (blanks, how_far > 72 ? 72 : (how_far < 0 ? 0 : how_far),
             1, out);
}

/*************************** put_line ***********************************/

static void put_line (out, line, additional_indent, min_level)
  FILE       *out;
  LINE_INFO  *line;    /* pointer for efficiency (is big object) */
  int        additional_indent;
  int        min_level;
{
  /* Writes the line 'line' to 'out' with the appropriate indentation 
     (which is the line's indentation plus additional_indent).
     If the current line numbering is not right a line_cmd is made before.
  */
  if (line->line_no != commanded_line_no + 1)
     line_cmd (out, line->line_no, line->indent + additional_indent,
               min_level);
  if (line->type == empty_line) {    /* for empty lines: nothing */
     putc ('\n', out);
     commanded_line_no++;
     return;
  }
  else if (option_small || (!option_indent && *line->start == '#')) {
     putc ('\n', out);
     commanded_line_no++;
  }
  else
     put_indent (out,
                 line->indent + additional_indent);  /* starts new line */
  assert (line->start != NULL);
  fputs (line->start, out);
#if debug
  fprintf (stdout, "put:\"%s\"\n", line->start);
#endif
}

/********************** refinement_nr ************************************/

static int refinement_nr (name)
  char *name;
{
  /* computes the number of a refinement from its name 'name'.
     Uses r from 0 to r_pos. Returns the number or -1 if undeclared.
     If the refinement is declared more than once an error message is 
     generated.
  */
  int i, match = -1, matches = 0;
  assert (name != NULL);
  for (i = 0; i < r_pos; i++)
      if (!strcmp (name, r[i].name)) {
         match = i;
         matches++;
      }
  if (matches > 1)
     error (Eref_multi_decl, r[match].name, refdecl_line_nr (match));
  return (match);
}

/********************* Auxiliary functions ******************************/

extern void copy_with_doubled_backslashes (string, copy)
  char *string, *copy;
{
  /* copies the string string to the location copy. All backslash
     characters (code 92) in string are copied twice, so for example
     "back\slas\\h" is copied to "back\\slas\\\\h".
     The purpose of this is to make the copy usable as a C string denoter,
     in which the backslash is interpreted as an escape symbol.
     No checks are made, thus there must be enough memory allocated
     to make the copy.
  */
  assert (string != NULL);
  assert (copy != NULL);
  while (*string != 0) {
     *copy = *string;
     string++;
     if (*copy == 92) {      /* is backslash ? */
       *(copy + 1) = 92;     /* then put another and */
       copy += 2;            /* proceed two bytes */
     }
     else                    /* else */
       copy++;               /* proceed one byte */
  }
  *copy = 0;
}


static void reset_l_r_s ()
{
  /* Sets LINE_INFO- and REF_INFO-Arrays 'l' and 'r' into 
     their empty state.
  */
  int i;
  for (i = 0; i <= r_pos; i++) { /* Alle Refinements loeschen */
      r[i].name  = NULL;
      r[i].calls = 0;
  }
  s = s_root;                    /* Zeilenspeicher loeschen */
  l_pos = r_pos = old_level = 0;
}


