/*************************************************************************
Project : C-Refine Precompiler
Module  : Line Scanner
Author  : Lutz Prechelt, Karlsruhe
Date    : 08.05.92  Version 17
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

#include <stdio.h>
#include <ctype.h>

#include "cr_decl.h"  /* globale Funktionen, Typen und Daten */

/******************* lokale Funktionen ************************************/

static void  push_refinement_name A(());
static int   normal_scanner A((LINE_INFO*));
static int   comment_scanner A((LINE_INFO*));
static int   text_scanner A((LINE_INFO*));
static int   char_scanner A((LINE_INFO*));
static int   preprocessor_scanner A((LINE_INFO*));

/*************************** defines ***************************************/

#define push(ch) ((*((s)++)) = (char)(ch))

#define leave_sequence    "leave"   /* e.g. "leave" (no leading blank!) */
#define leave_sequence_length  5

#define normal_end_of_line       1   /* real end of line */
#define continueing_end_of_line  2   /* logical end of line */
#define refinementsymbol_found   3
#define leave_keyword_found      4

/*********************** Lokale Daten ************************************/

/***** one line buffer *****/
static char *act;             /* current position in b */

/***** Control *****/
#if ansi
static int (*scanner) (LINE_INFO*);
#else
static int (*scanner) ();
#endif
/***** Status *****/
static int  level,                 /* brace-nesting */
            just_was, event,       /* event memory */
            semicolon_count,
            lines_in_denoter,
            old_indent;

/*********************** init_scanner ************************************/

extern void init_scanner ()
{
  /* Initializes the state of this module */
  level = 0;
  just_was = event = normal_end_of_line;
  scanner = normal_scanner;
}

/*********************** get_line ****************************************/

extern void get_line (fp, l, semicolons)
  FILE       *fp;
  LINE_INFO  *l;
  int        *semicolons;  /* is increased only ! */
{
  /* Reads on line from file fp and sets up l accordingly.
     The preprocessed line is copied to *s and s is increased appropriately.
     (A null terminator is appended.)
     The line is a complete line only, if there is no refinement involved 
     with that line:
     For refinement calls and refinement declaration headers a separate line
     is generated.
     At end of file stop_processing is set to true. Problems yield a 
     message and let 'errors' or 'warnings' increase.
     This function uses the option indicator variables, and the variables
     line_no and commanded_line_no.
  */
  charp old_s = s;    /* store s to compute length later */
  bool stop = false;
  bool three_witches;
  semicolon_count = 0;
  `init l;
  if (just_was == normal_end_of_line) {
     `read line;
     `get indent;
  }
  else {                       /* continued line */
     l->indent = old_indent;
     l->line_no = line_no;
  }
  `handle line;

`init l:
#if debug
  printf ("get_line:");
#endif
  l->level   = level;   /* level is defined as level at start of line! */
  l->start   = s;
  l->indent  = 0;
  l->type    = normal_line;

`read line:
  l->line_no = ++line_no;
  act = (char*)fgets (b, b_size, fp);  /* get next line*/
#if debug
  printf ("->%s<-", act == NULL ? "(NULL)" : (char*)act);
#endif
  if (act == NULL) {                /* check for EOF */
     stop_processing = true;
     l->type = empty_line;
     if (level > 0)
        error (Eeof_brace, NULL, line_no);
     if (scanner == comment_scanner)
        error (Eeof_comment, NULL, line_no);
     if (scanner == text_scanner)
        error (Eeof_string, NULL, line_no);
     if (scanner == char_scanner)
        error (Eeof_char, NULL, line_no);
     return;
  }

`get indent:
   while (!stop)
      if (*act == ' ') {
         l->indent++;
         act++;
      }
      else if (*act == TAB) {    /* expand Tabs */
         l->indent = (l->indent/tabsize + 1) * tabsize;
         act++;
      }
      else
         stop = true;
   old_indent = l->indent;    /* store for next call */

`handle line:
   three_witches = l->indent == 0 && level > 0 && scanner == normal_scanner;
   if (three_witches && (int)*act == refinementsymbol)
        `handle_refinement_declaration;
   else {
      `check column 0;
      if (just_was != refinementsymbol_found &&
          just_was != leave_keyword_found)
         event = (*scanner) (l);
      `handle_event;
      if (option_small || event == normal_end_of_line)
        `delete trailing blanks;
   }
   l->length = s - old_s;
   if (l->length == 0)
      l->type = empty_line;
   push (0);  /* String Terminator */
   `perhaps warn for level changes;
   assert (!(l->type == refdecl_line && semicolon_count != 0));
   *semicolons += semicolon_count;

`handle_refinement_declaration:
   act++;                     /* skip refinementsymbol */
   push_refinement_name ();
   if (*act != ':')
      error (Erefdecl_syntax, act, line_no);
   else
      act++;
   if (level > 1)
      error (Erefdecl_nested, NULL, line_no);
   l->type = refdecl_line;
   just_was = (*act == '\n' || *act == 0) ? normal_end_of_line :
                                            continueing_end_of_line;

`check column 0:
  if (three_witches && !iscntrl (*act) &&
      just_was != continueing_end_of_line &&
      *act != '}' && *act != '#' && *act != '*' && *act != '/')
     warning (Wcol0, act, line_no, 1);

`handle_event:
  if (event == refinementsymbol_found || event == leave_keyword_found)
     `handle refinementcall or leave;
  else
     just_was = normal_end_of_line;

`handle refinementcall or leave:
   if (s - old_s == 0) {        /* line empty */
      push_refinement_name ();
      l->type = event == leave_keyword_found ? leave_line : refcall_line;
      `skip semicolon and blanks;
      just_was = (*act == 0 || *act == '\n') ? normal_end_of_line :
                                               continueing_end_of_line;
   }
   else
      just_was = event;

`skip semicolon and blanks:
   if (*act == ';') {        /* skip semikolon if present */
      act++;
      semicolon_count++;
      if (l->type == refcall_line)
         l->type = refcallr_line;   /* note the removed ";" */
   }
   while (*(act++) == ' ')   /* skip following blanks */
           ;
   act--;                    /* recover char after last blank */

`delete trailing blanks:
   while (*(s-1) == ' ')      /* remove trailing blanks */
     s--;

`perhaps warn for level changes:
   int lev = level;
   if (lev < 0) {  /* Syntax error!  (or C-Refine does not work...) */
      if (option_anyway)
         error (Emany_braces, NULL, line_no);
      else
         fatal_error (Emany_braces, NULL, line_no);
   }
   else if (lev > 5 && level > l->level)
         warning (Wnesting, NULL, line_no, 3);
   else if (l->indent > 35 && `is refcall && !option_small)
         warning (Wmuch_indent, NULL, line_no, 3);

`is refcall:
   l->type == refcall_line || l->type == refcallr_line
}

/********************** push_refinement_name *******************************/

static void push_refinement_name ()
{
  /* reads input using 'act' and generates output using 's'.
     reads all following blanks, letters, digits and underscores (that is,
     stops on other characters) and generates from that a C identifier
     on the output by suppressing leading and trailing blanks.
     With option_ibmchars the Umlaute and Accentcharacters from the
     IBM International Charset are also allowed.
  */
#define is_legal(ch)  (isalnum(ch) || (ch) == ' ' || ch == '_' || \
                       (option_ibmchars && \
                           (((ch) >= 128 && (ch) <= 167)) || (ch) == 225))
  int ch;
  charp old_s = s, start = s;
  `suppress leading blanks;
  `copy legal chars;
  `suppress trailing blanks;
  assert (*(s-1) != ' ');
  assert (*old_s != ' ');
  assert (s - old_s >= 0);
  `change inner blanks to underlines;
  if (s - old_s == 0)
     error (Erefname_missing, act, line_no);

`suppress leading blanks:
   while (*act == ' ')               /* suppress leading blanks */
     act++;

`copy legal chars:
   do {                              /* copy legal chars */
     ch = *(act++);
     push (ch);
   }
   while (is_legal (ch));
   s--; act--;                       /* unpush illegal last char */

`suppress trailing blanks:
   while (*(s-1) == ' ' && s > old_s) /* suppress trailing blanks */
      s--;

`change inner blanks to underlines:
   for (start++; start < s; start++) /* change inner blanks to underlines */
     if (*start == ' ')
        *start = '_';
#undef is_legal
}

/***********************  S C A N N I N G  *********************************/

#define q    39      /* Quote */
#define dq   34      /* Double-Quote */

/*********************** normal_scanner ***********************************/

static int normal_scanner (l)
  LINE_INFO *l;
{
  /* Changes to reading comments, strings, quoted chars or preprocessor
     directives as necessary.
     Ends only at the end of a line.
     Tries to identify refinement calls and refinement declarations;
     in these cases the lines are reduced to only the refinement name.
  */
  register int ch;
  for (;;) {
    ch = *(act++);
    switch (ch) {
      case '\n':
      case 0   : return (normal_end_of_line);
      case '/' : if (*act == '*')              /* start of comment ? */
                    `handle normal comment;
                 else if (*act == '/' && option_cplusplus)
                    `handle doubleslash style comment;
                 else                           /* No --> normal */
                    push (ch);
                 break;
      case dq  : `handle text denoter;
      case q   : `handle char denoter;
      case '#' : `handle preprocessor directive;
      case ';' : semicolon_count++;
                 push (';');
                 break;
      case '{' : level++;
                 push (ch);
                 break;
      case '}' : level--;
                 push (ch);
                 break;
      default  :
                 if (ch == refinementsymbol)
                   `check for leave or refinement call;
                 else
                   push (ch);
    }
  }

`handle normal comment:
   if (!option_small) {
      push (ch); push ('*');
   }
   act++;
   scanner = comment_scanner;
   return ((*scanner) (l));

`handle doubleslash style comment:
   if (option_small)
      return (normal_end_of_line);   /* just pgnore rest of line */
   push (ch); push (ch);             /*  put  //   */
   act++;
   while (*act != '\n' && *act != 0)   /* put rest of line */
      push (*(act++));
   return (normal_end_of_line);

`handle text denoter:
   push (ch);
   scanner = text_scanner;
   lines_in_denoter = 0;
   return ((*scanner) (l));

`handle char denoter:
   push (ch);
   scanner = char_scanner;
   lines_in_denoter = 0;
   return ((*scanner) (l));

`handle preprocessor directive:
   push (ch);
   scanner = preprocessor_scanner;
   return ((*scanner) (l));

`check for leave or refinement call :
   /* Precondition: Refinement symbol found, 'act' is right behind it.
      if a 'leave' surrounded by blanks is found in front of the 
      refinement symbol, it and its blanks are stripped and 
      leave_keyword_found is returned.
      Otherwise refinementsymbol_found gemeldet is returned
   */
   charp old_s = s--;
   while (*s == ' ')
      s--;
   s++;
   if (!memcmp (s - leave_sequence_length, leave_sequence,
                leave_sequence_length)) {
      s -= leave_sequence_length;     /* Remove leave_sequence from Output */
      return (leave_keyword_found);
   }
   else {
      s = old_s;
      return (refinementsymbol_found);
   }
}

/********************* comment_scanner *************************************/

static int comment_scanner (l)
  LINE_INFO *l;
{
  /* Precondition: position is right behind a start of a comment 
     (which is already copied if not option_small is true)
     Postcondition: position is right after a comment end.
  */
  register int ch;
  for (;;) {
    ch = *(act++);
    switch (ch) {
      case '\n':
      case 0   : return (normal_end_of_line);
      case '*' : if (*act == '/')       /* end of comment : */
                    `handle comment end;
                 /* no break ! */
      default  : if (!option_small)
                    push (ch);
    }
  }

`handle comment end:
   if (!option_small) {
      push (ch); push ('/');
   }
   act++;
   scanner = normal_scanner;   /* change to normal scanner */
   return ((*scanner) (l));    /* and continue scanning */

}

/********************* text_scanner *************************************/

static int text_scanner (l)
  LINE_INFO *l;
{
  /* Precondition: position is right behind the (already copied) 
                   double quote that starts a string denoter 
                   (string literal)
     Postcondition:position is right behind the closing double
                   quote of a string denoter
  */
  register int ch;
  lines_in_denoter++;
  for (;;) {
    ch = *(act++);
    switch (ch) {
      case '\n':
      case 0   : return (normal_end_of_line);  /* allowed ??? */
      case dq  : `end of stringdenoter;
      case '\\': push (ch);
                 if (*act == dq || *act == '\\') {
                    push (*act);
                    act++;
                 }
                 break;
      default  : push (ch);
    }
 }

`end of stringdenoter:
   push (ch);
   if (lines_in_denoter > 1)
      warning (Wlong_string, act-1, line_no,
               lines_in_denoter > 5 ? 1 : 2);
   scanner = normal_scanner;
   return ((*scanner) (l));

}

/********************* char_scanner *************************************/

static int char_scanner (l)
  LINE_INFO *l;
{
  /* Is analogous to text scanner, but uses single quote instead of double
     quote.
  */
  int ch;
  lines_in_denoter++;
  for (;;) {
    ch = *(act++);
    switch (ch) {
      case '\n':
      case 0   : return (normal_end_of_line);  /* allowed ??? */
      case q   : `end of chardenoter;
      case '\\': push (ch);
                 if (*act == q || *act == '\\') {
                    push (*act);
                    act++;
                 }
                 break;
      default  : push (ch);
    }
  }

`end of chardenoter:
   push (ch);
   if (lines_in_denoter > 1)
      warning (Wlong_char, act-1, line_no,
               lines_in_denoter > 5 ? 1 : 2);
   scanner = normal_scanner;
   return ((*scanner) (l));

}

/********************* preprocessor_scanner ******************************/

static int preprocessor_scanner (l)
  LINE_INFO *l;
{
  /* Scans a line with a preprocessor directive on it.
     If this line contains a #line directive, reads the line number and
     file name and sets line_no and name_in accordingly. 
     This scanner is called immediately after the # has been seen.
     The line is copied verbatim.
  */
  int ch;
  ch = *(act++);
  `skip whitespace;
  `try to read line command;
  `copy rest of line;
  
`skip whitespace:
   for (;;) {
     switch (ch) {
       case ' ' :
       case TAB : push (ch);
                  break;
       case '\n':
       case 0   : scanner = normal_scanner;
                  return (normal_end_of_line);
       default  : leave `skip whitespace;
     }
     ch = *(act++);
   }

`try to read line command:
   /* precondition:  ch = <first nonwhitespace character after '#' >
      postcondition: ch = <first char not belonging to line command>
   */
   `read the command token;
   `skip whitespace;
   `read the line number;
   `skip whitespace;
   `read the filename;
  
`read the command token:
   /* ch = <first nonwhitespace character after '#' > */
   if (strncmp ("line", act, 4)) {
     if (!isdigit (ch))
       leave `try to read line command;
   }
   else {
     push (*(act++));
     push (*(act++));
     push (*(act++));
     push (*(act++));
     ch = *(act++);
     if (ch != ' ' && ch != TAB)  /* 'line<something> without whitespace */
       leave `try to read line command;
   }

`read the line number:
   int number = 0;
   while (isdigit (ch)) {
     push (ch);
     number = 10*number + (ch - '0');
     ch = *(act++);
   }
   line_no = commanded_line_no = number - 1;

`read the filename:
   bool has_quotes = ch == '\"';
   char *current_letter = name_in;
   if (has_quotes) {
     push (ch);
     ch = *(act++);
   }
   for (;;) {
     switch (ch) {
       case '\n':
       case 0   : `terminate name_in;
                  if (has_quotes)
                    warning (Eunterminated, name_in, line_no, warning_level);
                  scanner = normal_scanner;
                  return (normal_end_of_line);
       case dq  : `terminate name_in;
                  leave `read the filename;
       default  : push (ch);
                  *(current_letter++) = ch;
     }
     ch = *(act++);
   }

`terminate name_in:
   if (current_letter != name_in)  /* if new name present */
     *current_letter = 0;  /* terminate name */
   copy_with_doubled_backslashes (name_in, modified_name_in);

`copy rest of line:
   for (;;) {
     switch (ch) {
       case '\n':
       case 0   : scanner = normal_scanner;
                  return (normal_end_of_line);
       default  : push (ch);
     }
     ch = *(act++);
   }

}

