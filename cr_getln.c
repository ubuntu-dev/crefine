#line 1 "cr_getln.cr"
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
  {   /* init_l  (Level 1) */
#line 117 "cr_getln.cr"
#if debug
    printf ("get_line:");
#endif
    l->level   = level;   /* level is defined as level at start of line! */
    l->start   = s;
    l->indent  = 0;
    l->type    = normal_line;
  }
#line 106 "cr_getln.cr"
  if (just_was == normal_end_of_line) {
     {   /* read_line  (Level 1) */
#line 126 "cr_getln.cr"
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
     }
     {   /* get_indent  (Level 1) */
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
     }
#line 109 "cr_getln.cr"
  }
  else {                       /* continued line */
     l->indent = old_indent;
     l->line_no = line_no;
  }
  {   /* handle_line  (Level 1) */
#line 160 "cr_getln.cr"
  three_witches = l->indent == 0 && level > 0 && scanner == normal_scanner;
  if (three_witches && (int)*act == refinementsymbol)
       {   /* handle_refinement_declaration  (Level 2) */
#line 181 "cr_getln.cr"
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
       }
#line 163 "cr_getln.cr"
  else {
     {   /* check_column_0  (Level 2) */
#line 194 "cr_getln.cr"
     if (three_witches && !iscntrl (*act) &&
         just_was != continueing_end_of_line &&
         *act != '}' && *act != '#' && *act != '*' && *act != '/')
        warning (Wcol0, act, line_no, 1);
     }
#line 165 "cr_getln.cr"
     if (just_was != refinementsymbol_found &&
         just_was != leave_keyword_found)
        event = (*scanner) (l);
     {   /* handle_event  (Level 2) */
#line 200 "cr_getln.cr"
     if (event == refinementsymbol_found || event == leave_keyword_found)
        {   /* handle_refinementcall_or_leave  (Level 3) */
#line 206 "cr_getln.cr"
        if (s - old_s == 0) {        /* line empty */
           push_refinement_name ();
           l->type = event == leave_keyword_found ? leave_line : refcall_line;
           {   /* skip_semicolon_and_blanks  (Level 4) */
#line 217 "cr_getln.cr"
           if (*act == ';') {        /* skip semikolon if present */
              act++;
              semicolon_count++;
              if (l->type == refcall_line)
                 l->type = refcallr_line;   /* note the removed ";" */
           }
           while (*(act++) == ' ')   /* skip following blanks */
                   ;
           act--;                    /* recover char after last blank */
           }
#line 210 "cr_getln.cr"
           just_was = (*act == 0 || *act == '\n') ? normal_end_of_line :
                                                    continueing_end_of_line;
        }
        else
           just_was = event;
        }
#line 202 "cr_getln.cr"
     else
        just_was = normal_end_of_line;
     }
#line 169 "cr_getln.cr"
     if (option_small || event == normal_end_of_line)
       {   /* delete_trailing_blanks  (Level 2) */
#line 228 "cr_getln.cr"
       while (*(s-1) == ' ')      /* remove trailing blanks */
         s--;
       }
#line 171 "cr_getln.cr"
  }
  l->length = s - old_s;
  if (l->length == 0)
     l->type = empty_line;
  push (0);  /* String Terminator */
  {   /* perhaps_warn_for_level_changes  (Level 2) */
#line 232 "cr_getln.cr"
  int lev = level;
  if (lev < 0) {  /* Syntax error!  (or C-Refine does not work...) */
     if (option_anyway)
        error (Emany_braces, NULL, line_no);
     else
        fatal_error (Emany_braces, NULL, line_no);
  }
  else if (lev > 5 && level > l->level)
        warning (Wnesting, NULL, line_no, 3);
  else if (l->indent > 35 && (
#line 245 "cr_getln.cr"
  l->type == refcall_line || l->type == refcallr_line)
#line 241 "cr_getln.cr"
  && !option_small)
        warning (Wmuch_indent, NULL, line_no, 3);
  }
#line 177 "cr_getln.cr"
  assert (!(l->type == refdecl_line && semicolon_count != 0));
  *semicolons += semicolon_count;
  }
#line 246 "cr_getln.cr"
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
  {   /* suppress_leading_blanks  (Level 1) */
#line 275 "cr_getln.cr"
  while (*act == ' ')               /* suppress leading blanks */
    act++;
  }
  {   /* copy_legal_chars  (Level 1) */
  do {                              /* copy legal chars */
    ch = *(act++);
    push (ch);
  }
  while (is_legal (ch));
  s--; act--;                       /* unpush illegal last char */
  }
  {   /* suppress_trailing_blanks  (Level 1) */
  while (*(s-1) == ' ' && s > old_s) /* suppress trailing blanks */
     s--;
  }
#line 267 "cr_getln.cr"
  assert (*(s-1) != ' ');
  assert (*old_s != ' ');
  assert (s - old_s >= 0);
  {   /* change_inner_blanks_to_underlines  (Level 1) */
#line 291 "cr_getln.cr"
  for (start++; start < s; start++) /* change inner blanks to underlines */
    if (*start == ' ')
       *start = '_';
#undef is_legal
  }
#line 271 "cr_getln.cr"
  if (s - old_s == 0)
     error (Erefname_missing, act, line_no);
#line 295 "cr_getln.cr"
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
                    {   /* handle_normal_comment  (Level 1) */
#line 347 "cr_getln.cr"
                    if (!option_small) {
                       push (ch); push ('*');
                    }
                    act++;
                    scanner = comment_scanner;
                    return ((*scanner) (l));
                    }
#line 321 "cr_getln.cr"
                 else if (*act == '/' && option_cplusplus)
                    {   /* handle_doubleslash_style_comment  (Level 1) */
#line 355 "cr_getln.cr"
                    if (option_small)
                       return (normal_end_of_line);   /* just pgnore
rest of line */
                    push (ch); push (ch);             /*  put  //   */
                    act++;
                    while (*act != '\n' && *act != 0)   /* put rest of line */
                       push (*(act++));
                    return (normal_end_of_line);
                    }
#line 323 "cr_getln.cr"
                 else                           /* No --> normal */
                    push (ch);
                 break;
      case dq  : 
      {   /* handle_text_denoter  (Level 1) */
#line 364 "cr_getln.cr"
      push (ch);
      scanner = text_scanner;
      lines_in_denoter = 0;
      return ((*scanner) (l));
      }
#line 327 "cr_getln.cr"
      case q   : 
      {   /* handle_char_denoter  (Level 1) */
#line 370 "cr_getln.cr"
      push (ch);
      scanner = char_scanner;
      lines_in_denoter = 0;
      return ((*scanner) (l));
      }
#line 328 "cr_getln.cr"
      case '#' : 
      {   /* handle_preprocessor_directive  (Level 1) */
#line 376 "cr_getln.cr"
      push (ch);
      scanner = preprocessor_scanner;
      return ((*scanner) (l));
      }
#line 329 "cr_getln.cr"
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
                   {   /* check_for_leave_or_refinement_call  (Level 1) */
#line 381 "cr_getln.cr"
                   /* Precondition: Refinement symbol found, 'act' is
right behind it.
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
                      s -= leave_sequence_length;     /* Remove
leave_sequence from Output */
                      return (leave_keyword_found);
                   }
                   else {
                      s = old_s;
                      return (refinementsymbol_found);
                   }
                   }
#line 341 "cr_getln.cr"
                 else
                   push (ch);
    }
  }
#line 400 "cr_getln.cr"
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
                    {   /* handle_comment_end  (Level 1) */
#line 426 "cr_getln.cr"
                    if (!option_small) {
                       push (ch); push ('/');
                    }
                    act++;
                    scanner = normal_scanner;   /* change to normal scanner */
                    return ((*scanner) (l));    /* and continue scanning */
                    }
#line 419 "cr_getln.cr"
                 /* no break ! */
      default  : if (!option_small)
                    push (ch);
    }
  }
#line 433 "cr_getln.cr"
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
      case dq  : 
      {   /* end_of_stringdenoter  (Level 1) */
#line 465 "cr_getln.cr"
      push (ch);
      if (lines_in_denoter > 1)
         warning (Wlong_string, act-1, line_no,
                  lines_in_denoter > 5 ? 1 : 2);
      scanner = normal_scanner;
      return ((*scanner) (l));
      }
#line 454 "cr_getln.cr"
      case '\\': push (ch);
                 if (*act == dq || *act == '\\') {
                    push (*act);
                    act++;
                 }
                 break;
      default  : push (ch);
    }
 }
#line 472 "cr_getln.cr"
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
      case q   : 
      {   /* end_of_chardenoter  (Level 1) */
#line 501 "cr_getln.cr"
      push (ch);
      if (lines_in_denoter > 1)
         warning (Wlong_char, act-1, line_no,
                  lines_in_denoter > 5 ? 1 : 2);
      scanner = normal_scanner;
      return ((*scanner) (l));
      }
#line 490 "cr_getln.cr"
      case '\\': push (ch);
                 if (*act == q || *act == '\\') {
                    push (*act);
                    act++;
                 }
                 break;
      default  : push (ch);
    }
  }
#line 508 "cr_getln.cr"
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
  {   /* skip_whitespace  (Level 1) */
#line 528 "cr_getln.cr"
  for (;;) {
    switch (ch) {
      case ' ' :
      case TAB : push (ch);
                 break;
      case '\n':
      case 0   : scanner = normal_scanner;
                 return (normal_end_of_line);
      default  : 
  goto skip_whitespace_1;
#line 537 "cr_getln.cr"
    }
    ch = *(act++);
  }
skip_whitespace_1: ;
  }
  {   /* try_to_read_line_command  (Level 1) */
#line 542 "cr_getln.cr"
  /* precondition:  ch = <first nonwhitespace character after '#' >
     postcondition: ch = <first char not belonging to line command>
  */
  {   /* read_the_command_token  (Level 2) */
#line 552 "cr_getln.cr"
  /* ch = <first nonwhitespace character after '#' > */
  if (strncmp ("line", act, 4)) {
    if (!isdigit (ch))
  goto try_to_read_line_command_1;
  }
  else {
    push (*(act++));
    push (*(act++));
    push (*(act++));
    push (*(act++));
    ch = *(act++);
    if (ch != ' ' && ch != TAB)  /* 'line<something> without whitespace */
  goto try_to_read_line_command_1;
  }
  }
  {   /* skip_whitespace  (Level 2) */
#line 528 "cr_getln.cr"
  for (;;) {
    switch (ch) {
      case ' ' :
      case TAB : push (ch);
                 break;
      case '\n':
      case 0   : scanner = normal_scanner;
                 return (normal_end_of_line);
      default  : 
  goto skip_whitespace_2;
#line 537 "cr_getln.cr"
    }
    ch = *(act++);
  }
skip_whitespace_2: ;
  }
  {   /* read_the_line_number  (Level 2) */
#line 568 "cr_getln.cr"
  int number = 0;
  while (isdigit (ch)) {
    push (ch);
    number = 10*number + (ch - '0');
    ch = *(act++);
  }
  line_no = commanded_line_no = number - 1;
  }
  {   /* skip_whitespace  (Level 2) */
#line 528 "cr_getln.cr"
  for (;;) {
    switch (ch) {
      case ' ' :
      case TAB : push (ch);
                 break;
      case '\n':
      case 0   : scanner = normal_scanner;
                 return (normal_end_of_line);
      default  : 
  goto skip_whitespace_3;
#line 537 "cr_getln.cr"
    }
    ch = *(act++);
  }
skip_whitespace_3: ;
  }
  {   /* read_the_filename  (Level 2) */
#line 577 "cr_getln.cr"
  bool has_quotes = ch == '\"';
  char *current_letter = name_in;
  if (has_quotes) {
    push (ch);
    ch = *(act++);
  }
  for (;;) {
    switch (ch) {
      case '\n':
      case 0   : 
      {   /* terminate_name_in  (Level 3) */
#line 600 "cr_getln.cr"
      if (current_letter != name_in)  /* if new name present */
        *current_letter = 0;  /* terminate name */
      copy_with_doubled_backslashes (name_in, modified_name_in);
      }
#line 587 "cr_getln.cr"
                 if (has_quotes)
                   warning (Eunterminated, name_in, line_no, warning_level);
                 scanner = normal_scanner;
                 return (normal_end_of_line);
      case dq  : 
      {   /* terminate_name_in  (Level 3) */
#line 600 "cr_getln.cr"
      if (current_letter != name_in)  /* if new name present */
        *current_letter = 0;  /* terminate name */
      copy_with_doubled_backslashes (name_in, modified_name_in);
      }
  goto read_the_filename_1;
#line 593 "cr_getln.cr"
      default  : push (ch);
                 *(current_letter++) = ch;
    }
    ch = *(act++);
  }
read_the_filename_1: ;
  }
try_to_read_line_command_1: ;
  }
  {   /* copy_rest_of_line  (Level 1) */
#line 605 "cr_getln.cr"
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
}

