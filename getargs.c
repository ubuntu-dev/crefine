/*************************************************************************
Module  : getargs -- command line option processor
Author  : Lutz Prechelt, Karlsruhe
Date    : 11.06.91  Version 2
Compiler: should be portable
          tried: MS-DOS 3.2, Microsoft C 5.0 (ANSI-C)
          SUN-OS 4.03, 4.1 (K&R C); GNU gcc 1.39
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
#if 0

Varianten:
#define deutsch 1   waehlt deutsche Meldungen statt englische
#define ansi    1   waehlt Funktionsdefinitionen mit Parameterliste
                    statt ohne
#define ms_dos  1   erlaubt "/" als Optionszeichen zusaetzlich zu "-"

Dies ist ein Utility zum bequemeren Auswerten von Optionen auf der
Kommandozeile (d.h. mit argc, argv).

Es wird dazu vom Benutzer eine Tabelle ("argtab") aufgestellt, in der zu
jeder Option der Optionsbuchstabe, ein Argumententyp 
(BOOL, CHAR, INT, STRING) und eine Variable angegeben werden,
in der das Resultat abgelegt werden soll.
Ferner kann man hier noch einen Kommentarstring zu jeder Option angeben.

Die Funktion getargs (&argc, argv, argtab, argtabsize) bearbeitet nun den
argv Vektor derart, dass alle Optionen, die in argtab angegeben sind, gemaess
ihrem Eintrag behandelt werden und aus argv entfernt (dazu wird argc und argv
modifiziert, jedoch nur Pointer veraendert, keine argv-Eintraege selbst)
Es duerfen beim Aufruf die Optionen beliebig mit den uebrigen Parametern
vermischt und mehrere Optionen hinter einem Optionssymbol angegeben
werden.

Es werden unbekannte Optionen festgestellt und angemeckert (nach stderr)
Als Resultat wird die Anzahl angemeckerter Optionen geliefert.

Die Funktion print_usage (program_name, usage_text, argtab, argtabsize)
gibt unter Benutzung der Kommentarstrings aus argtab eine Kurzbeschreibung zum
korrekten Aufruf aller Optionen und des Gesamtprogramms nach stderr aus.

Beispiel:
  #include <getargs.h>
  int a = 1, b = 'B', c;
  char *d = "";
  ARG argtab[] = { {'a', BOOLEAN,  &a,       "use alternate mode" },
                   {'b', CHARACTER,&b,       "Character for boldface" },
                   {'c', INTEGER,  &c,       "count of pages to print" },
                   {'d', STRING  , (int*)&d, "File description" } };
  void main (int argc, char **argv) {
     if (getargs (&argc, argv, argtab, ARGTABSIZE (argtab)))
        print_usage (argv[0], "[options] filetoprint [options]", argtab,
                     ARGTABSIZE (argtab));
  }

Enthalte argtab also die Liste der Optionen fuer das Programm "fileprt", das
(wie in print_usage angegeben) noch einen Dateinamen als weiteren Parameter
verlangt.

Dann waeren korrekte Aufrufe etwa:

fileprt -a file
fileprt -a+ file -bB
fileprt -c14 file -dMyFile
fileprt file -abBc14

Verkehrt waere dagegen zum Beispiel:

fileprt -dMy File file       weil <File> ein eigenes Argument ist
fileprt -p file              weil es die Option p nicht gibt
fileprt -bx28 file           weil CHAR immer nur ein Zeichen umfasst

Dieser letzte Aufruf haette im Beispiel ungefaehr folgenden Output nach
stderr zur Folge:

Unbekannte Option : -bx28
usage: C:\MYDIR\FILEPRT.EXE [options] filetoprint [options]
Erlaubte Optionen sind:
-a<+/-> use alternate mode                       (Wert: TRUE)
-b<ch>  Character for boldface                   (Wert: B)
-c<num> count of pages to print                  (Wert: 0)
-d<str> File description                         (Wert: "")

Der genaue Grund fuer die Fehlermeldung ist, dass der Interpreter der Option
b den Wert x zuweist und dann nach der (nicht auffindbaren) Option 2 sucht !

Die "Wert"-Angaben beim print_usage entstehen aus den Vorbelegungen der
Variablen a, b, c, d;

#endif

/**************************************************************************/

#include "std.h"
#include <stdio.h>
#include <ctype.h>
#include "getargs.h"

#if deutsch
#define ERRMSG         "Unbekannte Option"
#define USAGE          "Aufruf"
#define VALID_OPT_ARE  "Erlaubte Optionen sind"
#else
#define ERRMSG         "unknown option"
#define USAGE          "usage"
#define VALID_OPT_ARE  "valid options are"
#endif

#if ms_dos
#define is_option(a) (a == '-' || a == '/')   /* MS-DOS option marker */
#else
#define is_option(a) (a == '-')   /* option marker for Unix etc. */
#endif

static int  set_argument A((ARG* entry, char **optiontext));
static ARG* find_argument A((int optionname, ARG *argtab, int argtablength));
static int  stoi A((char ** linep));

/*---------------------------------------------------------
 *    getargs
 *---------------------------------------------------------*/

extern int getargs (argc, argv, tabp, tabsize)
  int  *argc;    /* changed */
  char **argv;   /* changed */
  ARG  *tabp;
  int  tabsize;
{
  /* Hauptroutine. Wertet alle Argumente in argv bis argc aus:
     Es duerfen Optionen (erkannt am ersten Zeichen gemaess Makro is_option)
     und andere Argumente beliebig gemischt auftreten.
     Optionen werden in tabp gesucht und durch Linksschieben der restlichen
     Argumente aus argv entfernt.
     Sie werden gemaess ihrem tabp-Eintrag behandelt:
     Bei Nichtvorhandensein oder unerlaubten Werten wird gemeckert,
     andernfalls die betreffende Variable mit dem Wert der Option bestueckt.
     Nur Nicht-Optionen bleiben in argv enthalten, ihre Anzahl wird in argc
     vermerkt.
     Das Resultat ist die Anzahl angemeckerter Optionen.
  */
  int    nargc;   /* remaining argc after removing options */
  char **nargv, *p;
  int    errors = 0, error;
  ARG   *argp;
  for (nargv = ++argv, nargc = 1 /* keep name */; --(*argc) > 0; argv++) {
    if (!is_option (**argv)) {
       *nargv++ = *argv;        /* No Option -> keep it */
       nargc++;
    }
    else {
       p = (*argv) + 1;         /* Option -> handle it */
       while (*p) {
          error = 0;
          /* One Optionsign can have multiple Options following */
          if (argp = find_argument ((int)*p, tabp, tabsize)) /* if exists */
             error = !set_argument (argp, &p);          /* then read it */
          if (!argp || error) {   /* if not exists or invalid value */
             fprintf (stderr, "%s : %s\n", ERRMSG, *argv);   /* then #$% */
             errors++;
             break;
          }
       }
    }
  }
  *argc = nargc;
  return (errors);
}

/*---------------------------------------------------------
 *    set_argument
 *---------------------------------------------------------*/

static int set_argument (argp, linep)
  ARG  *argp;
  char **linep;
{
  /* Holt den Wert fuer die (bereits in argtab als vorhanden erkannte)
     Option in die zugehoerige in argtab angegebene Variable.
     Es wird linep so weit weitergeschaltet, wie Daten zur Bestimmung der
     Variablen entnommen werden, d.h. bei STRING auf NULL, bei CHARACTER
     um 1 Zeichen, bei INTEGER bis zum Ende der Zahl
     (oder gar nicht, falls keine kommt) und bei BOOLEAN um 0 oder 1,
     je nachdem ob ein + oder - angegeben wurde oder nicht.
     Liefert 0 falls ein nicht erlaubter Wert festgestellt wurde, sonst 0.
  */
#define p (argp->variable)
  char *oldp;
  ++(*linep);   /* skip Optionname */
  oldp = *linep;
  switch (argp->type) {
        case INTEGER:
              *p = stoi (linep);
              return (oldp != *linep);
        case BOOLEAN:
              if (**linep == '-' || **linep == '+') {  /* + or - given */
                 *p = (**linep == '+');
                 ++(*linep);              /* then take it and evaluate */
              }
              else                        /* else assume TRUE */
                 *p = 1;
              return (1);
        case CHARACTER:
              *p = (int)**linep;
              ++(*linep);      /* go on one char */
              return (*p != 0);
        case STRING:
              *(char **)p = *linep;
              *linep = "";   /* take all the rest */
              return (1);
  }
#undef p
}

/*---------------------------------------------------------
 *    find_argument
 *---------------------------------------------------------*/

static ARG* find_argument (optionname, tabp, tabsize)
  int  optionname;
  ARG *tabp;
  int  tabsize;
{
  for (; --tabsize >= 0; tabp++)
      if (tabp->arg == optionname)
         return (tabp);
  return (0);  /* not found */
}

/*---------------------------------------------------------
 *    print_usage
 *---------------------------------------------------------*/

extern void print_usage (progname, usage, tabp, tabsize)
  char *progname, *usage;
  ARG  *tabp;
  int  tabsize;
{
  /* Druckt die Optionsbeschreibungen laut tabp incl. momentaner Werte der
     zugeh. Variablen sowie zuvor eine "usage:" Zeile mit den Texten
     progname und usage.
  */
  char *p;
  int  i;
  fprintf (stderr, "\n%s: %s %s\n%s:\n", USAGE, progname,
	   usage, VALID_OPT_ARE);
  for (i = 0; i < tabsize; i++, tabp++) {
      fprintf (stderr, "-%c", tabp->arg);
      p = tabp->errmsg;
      switch (tabp->type) {
            case INTEGER:
                  fprintf (stderr, "<num> %-45s (%d)\n",
                           p, *(tabp->variable));
                  break;
            case BOOLEAN:
                  fprintf (stderr, "<+/-> %-45s (%s)\n",
                           p, *(tabp->variable) ? "TRUE" : "FALSE");
                  break;
            case CHARACTER:
                  fprintf (stderr, "<ch>  %-45s (%c)\n",
                           p, (char)*(tabp->variable));
                  break;
            case STRING:
                  fprintf (stderr, "<str> %-45s (\"%s\")\n",
                           p, *(char **)tabp->variable);
                  break;
            }
      }
}


/********************* Hilfsfunktionen ************************************/


/*---------------------------------------------------------
 *    stoi
 *---------------------------------------------------------*/

static int stoi (instr)
  register char **instr;
{
  int  num = 0;
  int  sign = 0;  /* Betrag und neg. Vorzeichen des Resultats */
  char *str = *instr;
  while (isspace (*str))
        str++;
  if (*str == '-') {
     sign = -1;
     str++;
  }
  if (*str == '0') {
     ++str;
     if (toupper (*str) == 'X') {
        str++;
        while(isxdigit(*str)) {
              num *= 16;
              num += isdigit (*str) ? *str - '0' 
		                    : toupper (*str) - 'A'+ 10;
              str++;
        }
     }
     else {
        while ('0' <= *str && *str <= '7') {
              num *= 8;
              num += *str++ - '0';
        }
     }
  }
  else {
     while (isdigit (*str)) {
           num *= 10;
           num += *str++ - '0';
     }
  }
  *instr = str;
  return (sign ? -num : num);
}

/************* Hauptprogramm zum Testen **********************************/

#if 0

  int a, b, c;
  char *d;
  ARG argtab[] = { {'a', BOOLEAN, &a, "Option a" },
                   {'b', CHARACTER,&b, "Option b" },
                   {'c', INTEGER, &c, "Option c" },
                   {'d', STRING  , (int*)printf, "Option d" } };

void main (int argc, char **argv)
{
  int i, n;
  printf ("Argumentpointer argv[i]: ");
  for (i = 0; i < argc; i++)
      printf ("%d ", (int)argv[i]);
  printf ("\n");
  n = getargs (&argc, argv, argtab, ARGTABSIZE (argtab));
  if (n)
     print_usage (argv [0], "rabarber", argtab, ARGTABSIZE (argtab));
  printf ("\n\n RESULTATE (%d): a = %d, b = %c, c = %d, d = \"%s\"\n\n",
          n, a, (char)b, c, d);
  printf ("ARGV: ");
  for (i = 0; i < argc; i++)
      printf ("<%s> ", argv[i]);
}

#endif

