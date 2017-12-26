/*************************************************************************
Project : C-Refine Precompiler
Module  : Message texts
Author  : Lutz Prechelt, Karlsruhe
Date    : 29.01.91  Version 15
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

charp
  usagestring      [] = {
   "[optionen] eingabedatei [ausgabedatei] [optionen]",
   "[options] inputfile [outputfile] [options]",
   "[optionen] eingabedatei [ausgabedatei] [optionen]"
                        },
  versionstring    [] = {
   "Version 2.4   Januar '91",
   "Version 2.4   January '91",
   "Version 2.4   Jaenner '91"
                        },
  expirestring     [] = {
   "gueltig bis",
   "expires",
   "entschlaeft am"
                        },
  Terror           [] = {
   "Fehler",
   "error",
   "Fehler"
                        },
  Twarning         [] = {
   "Warnung",
   "warning",
   "Warnung"
                        },
  Tfatal_error     [] = {
   "Fataler Fehler",
   "fatal error",
   "fatal, fatal"
                        },
  Tlistline        [] = {
   "   /* %s  (Ebene %d) */",
   "   /* %s  (Level %d) */",
   "   /* %s  (Ebene %d) */"
                        },
  Tnear            [] = {
   "bei",
   "near",
   "bei"
                        },
  Thas_expired     [] = {
   "--- Diese (Test-)Version ist nicht mehr gueltig ---",
   "--- this (test-)version has expired ---",
   "--- Diese (Test-)Version ist friedlich entschlafen ---"
                        },
  Wempty_function  [] = {
   "Leere Funktion (aber mit Refinements)",
   "function is empty (but has refinements)",
   "Viel Gelaber und nix dahinter"
                        },
  Wlong_ref        [] = {
   "Sehr langes Refinement",
   "very long refinement",
   "Ist das Refinement nicht a bisserl arg lang ?"
                        },
  Wref_often_used  [] = {
   "Refinement ueber 5 mal benutzt",
   "refinement used more than 5 times",
   "hektisches ich-bin-ueberall-Refinement"
                        },
  Wref_unused      [] = {
   "Refinement wird nicht benutzt",
   "refinement unused",
   "ein Refinement steht im Walde...."
                        },
  Wref_empty       [] = {
   "Leeres Refinement",
   "refinement is empty",
   "Was will uns dieses Refinement sagen ?"
                        },
  Wcol0            [] = {
   "Unerwartete Zeichen in Spalte 0",
   "unexpected character in column 0",
   "Was tobt denn da in Spalte 0 rum ?"
                        },
  Wnesting         [] = {
   "Sehr tiefe Blockverschachtelung",
   "deep block-nesting",
   "SCHEISS STIL.  Verschachtele doch nicht so tief"
                        },
  Wmuch_indent     [] = {
   "Einrueckung evtl. nicht mehr darstellbar",
   "perhaps indentation will be reduced",
   "Das koennte ueber meinen (rechten) Horizont gehen"
                        },
  Wlong_string     [] = {
   "Langer Stringdenoter hier zu Ende",
   "end of long string denoter here",
   "Laaaaaanger Stringdenoter hoert hier auf (na endlich)"
                        },
  Wlong_char       [] = {
   "Langer Character-Denoter hier zu Ende",
   "end of long character denoter here",
   "Laaaaanger Character-Denoter hoert hier auf"
                        },
  Eopen            [] = {
   "\nKann Datei \"%s\" nicht oeffnen\n",
   "\nerror opening \"%s\"\n",
   "\nNa wo isse denn, die Datei \"%s\" ?\n"
                        },
  Ereadinput       [] = {
   "Kann Eingabedatei nicht lesen",
   "can't read inputfile",
   "akutes Eingabedateileseunvermoegen"
                        },
  Ewriteoutput     [] = {
   "Kann Ausgabedatei nicht beschreiben",
   "can't write outputfile",
   "akute Ausgabedateischreiblaehmung"
                        },
  Elevel0_ref      [] = {
   "Refinements ausserhalb einer Funktion gibts nicht",
   "refinements illegal outside functions",
   "Ein Refinement ?  Hier draussen in der Wueste ?"
                        },
  Elines_in_func   [] = {
   "Funktion hat zuviele Zeilen",
   "too many lines in function",
   "Soooo viele Zeilen in einer Funktion haelt ja kein Computer aus"
                        },
  Ebytes_in_func   [] = {
   "Funktion zu lang (in Bytes)",
   "too many bytes in function",
   "Tolle Funktion.  Aber, in Bytes betrachtet, entschieden zu gross."
                        },
  Eref_not_decl    [] = {
   "Refinement nicht deklariert",
   "undeclared refinement",
   "Wer oder was soll das sein ?"
                        },
  Erecursive_ref   [] = {
   "Rekursiver Refinementaufruf",
   "recursive call to refinement",
#define _rum_und_rum   "und rum und rum und rum und rum und rum und rum\
 und rum und rum (rekursives Refinement)"
   _rum_und_rum
                        },
  Eunknown_leave   [] = {
   "zu verlassendes Refinement unbekannt",
   "refinement to leave is unknown",
   "WAS soll da verlassen werden ???"
                        },
  Eleave_unpresent [] = {
#define _unpresent "Wir sind nicht in diesem Refinement, also kann es\
 auch nicht verlassen werden",
   _unpresent
   "you are not within this refinement, thus it's impossible to leave it",
#define _wenn_du   "Wenn Du denkst zu sein, wo ich denke (und du\
 schreibst), dass du zu sein denkst, denkst du falsch."
   _wenn_du
                        },
  Eref_multi_decl  [] = {
   "Refinement mehrfach deklariert",
   "refinement declared more than once",
   "Langweilig!  Das Refinement gibts doch laengst"
                        },
  Ememory          [] = {
   "\n***** Speichermangel *****\n",
   "\n***** not enough memory *****\n",
   "\n  Des Bissele Speische is mi abe zu wenisch !\n"
                        },
  Eeof_brace       [] = {
   "Dateiende bei offener geschweifter Klammer",
   "EOF within block",
   "In der U-Bahn geboren, oder was ?   Mach erst mal alle Bloecke zu."
                        },
  Eeof_comment     [] = {
   "Dateiende im Kommentar",
   "EOF in comment",
#define _so_toll   "So toll sind Deine Kommentare nun auch nicht, dass\
 sie bis ans Dateiende gehen sollten"
   _so_toll
                        },
  Eeof_string      [] = {
   "Dateiende im Stringdenoter",
   "EOF in string denoter",
   "Hoerst Du immer mitten im Satz auf ?  Da ist noch ein String offen"
                        },
  Eeof_char        [] = {
   "Dateiende im Character-Denoter",
   "EOF in character denoter",
   "EOF in Character-Denoter.  Demnaechst halbieren sie noch die Bits...."
                        },
  Erefdecl_syntax  [] = {
   "falsche Syntax fuer Refinementdeklaration",
   "illegal syntax for refinement declaration",
   "Es begann wie eine Refinementdeklaration. Aber dann....."
                        },
  Erefdecl_nested  [] = {
   "Mehr als ein Block offen bei Refinementdeklaration",
   "refinement declaration in nested block",
#define _du_bist "Du bist an dieser Stelle viel zu tief in dein Programm\
 verstrickt, um ein Refinement zu deklarieren"
   _du_bist
                        },
  Emany_braces     [] = {
   "Zuviel schliessende Blockklammern",
   "too many closing braces",
#define _aufmachen   "Wenn Du jetzt noch eine Blockklammer aufmachst,\
 dann sind gar keine mehr offen"
   _aufmachen
                        },
  Erefname_missing [] = {
   "Refinementsymbol ohne Refinementname",
   "refinement symbol found without refinement name",
   "Das Refinementsymbol fuehlt sich einsam so ganz ohne Name"
                        },
  Emany_errors     [] = {
   "Zu viele Fehler. Uebersetzung wird abgebrochen.",
   "too many errors,  compilation stopped",
   "Fiel zufiele Feeler"
                        },
  Emany_warnings   [] = {
   "Zu viele Warnungen. Uebersetzung wird abgebrochen",
   "too many warnings, compilation stopped",
   "(So viele Warnungen sind mir nicht geheuer)"
                        };


