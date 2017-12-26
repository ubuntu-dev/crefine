#line 1 "cr_talk.cr"
/*************************************************************************
Project : C-Refine Precompiler
Module  : Output functions for messages
Author  : Lutz Prechelt, Karlsruhe
Date    : 11.06.91  Version 16
Compiler: ANSI C, C-Refine
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
#include <string.h>

#include "cr_decl.h"   /* global Functions, Types and Data */
#include "cr_texts.h"  /* message texts */

static void error_message A((charp[], charp[], charp, int));

/************************* cout *******************************************/

extern void cout (i)
  int i;
{
  /* Writes number i on stderr (which is assumed to be a screen)
     with length five and positions the cursor back to the starting
     position with five backspaces.
     This is not very fast, but that doesn't matter.
  */
  fprintf (stderr, "%5d\b\b\b\b\b", i);
}

/************************ error *******************************************/

static void error_message (type, message, object, line)
  charp type[], message[];
  charp object;
  int   line;
{
  fprintf (stdout, "\"%s\", line %d, %s: %s\n", name_in,
           line, type[msg_type], message[msg_type]);
  if (object != NULL) {
    char *nl = strchr (object, '\n');
    if (nl != NULL)     /* remove newline from object */
      *nl = 0;
   fprintf (stdout, "    %s \"%s\"\n", Tnear[msg_type], object);
   if (nl != NULL)
     *nl = '\n';
  }
}


extern void error (message, object, line)
  charp message[];
  charp object;
  int   line;
{
  error_message (Terror, message, object, line);
  if (++errors > maxerrors)
     fatal_error (Emany_errors, "(-----ManteldesSchweigens-----)", line);
  error_in_this_function = true;
}

/************************ fatal_error *************************************/

extern void fatal_error (message, object, line)
  charp message[];
  charp object;
  int   line;
{
  error_message (Tfatal_error, message, object, line);
  stop_processing = true;
  errors++;
}

/************************ warning *****************************************/

extern void warning (message, object, line, level)
  charp message[];
  charp object;
  int   line, level;
{
  if (!error_in_this_function && level <= warning_level) {
     /* don't be too verbose */
     error_message (Twarning, message, object, line);
     if (++warnings > maxwarnings)
        fatal_error (Emany_warnings, NULL, line);
  }
}

