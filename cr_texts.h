/*************************************************************************
Project : C-Refine Precompiler
Module  : Message texts
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

charp
  usagestring      [] = {
   "[option | eingabedatei]...",
   "[options | inputfile]...",
   "[option | eingabedatei]...",
   "[options | inputfile]..."
                        },
  versionstring    [] = {
   "Version 3.0   Juni '92",
   "Version 3.0   June '92",
   "Version 3.0   Juniieh '92",
   "Version three-dot-ooh  June '92"
                        },
  Terror           [] = {
   "Fehler",
   "error",
   "Faehler",
   "erorr"
                        },
  Twarning         [] = {
   "Warnung",
   "warning",
   "Warnung",
   "warning"
                        },
  Tfatal_error     [] = {
   "Fataler Fehler",
   "fatal error",
   "fatal, fatal",
   "!@#$^&*(;*^"
                        },
  Tlistline        [] = {
   "   /* %s  (Ebene %d) */",
   "   /* %s  (Level %d) */",
   "   /* %s  (Ebene %d) */",
   "   /* %s  (Level %d) */"
                        },
  Tnear            [] = {
   "bei",
   "near",
   "bei",
   "near"
                        },
  Wempty_function  [] = {
   "Leere Funktion (aber mit Refinements)",
   "function is empty (but has refinements)",
   "Viel Gelaber und nix dahinter",
   "You are refining NOTHING"
                        },
  Wlong_ref        [] = {
   "Sehr langes Refinement",
   "very long refinement",
   "Ist das Refinement nicht a bisserl arg lang ?",
   "veeeryyy looong reeefiiineeemeeent"
                        },
  Wref_often_used  [] = {
   "Refinement ueber 5 mal benutzt",
   "refinement used more than 5 times",
   "hektisches ich-bin-ueberall-Refinement",
   "Superman-refinement (is almost everywhere at the same time)"
                        },
  Wref_unused      [] = {
   "Refinement wird nicht benutzt",
   "refinement unused",
   "ein Refinement steht im Walde....",
   "I feel sohoho looooooonely, nobody wants me!"
   
                        },
  Wref_empty       [] = {
   "Leeres Refinement",
   "refinement is empty",
   "Was will uns dieses Refinement sagen ?",
   "the set of statements in this refinement is the empty set"
                        },
  Wcol0            [] = {
   "Unerwartete Zeichen in Spalte 0",
   "unexpected character in column 0",
   "Was tobt denn da in Spalte 0 rum ?",
   "Hmm ?  Such stuff in column 0 ?  Awkward!",
                        },
  Wnesting         [] = {
   "Sehr tiefe Blockverschachtelung",
   "deep block-nesting",
   "SCHEISS STIL.  Verschachtele doch nicht so tief",
   "You damn brain-dead C programmer: THOU SHALT NOT NEST DEEP !"
   
                        },
  Wmuch_indent     [] = {
   "Einrueckung evtl. nicht mehr darstellbar",
   "perhaps indentation will be reduced",
   "Das koennte ueber meinen (rechten) Horizont gehen",
   "Uff, maybe I won't be able to indent that right-wing stuff as you want."
   
                        },
  Wlong_string     [] = {
   "Langer Stringdenoter hier zu Ende",
   "end of long string denoter here",
   "Laaaaaanger Stringdenoter hoert hier auf (na endlich)",
   "I really thought this string denoter would never end..."
                        },
  Wlong_char       [] = {
   "Langer Character-Denoter hier zu Ende",
   "end of long character denoter here",
   "Laaaaanger Character-Denoter hoert hier auf",
   "The thing ending here is quite long for a single character..."
                        },
  Eopen            [] = {
   "\nKann Datei \"%s\" nicht oeffnen\n",
   "\nerror opening \"%s\"\n",
   "\nNa wo isse denn, die Datei \"%s\" ?\n",
   "\nAm I blind? Where is \"%s\"\n"
                        },
  Ereadinput       [] = {
   "Kann Eingabedatei nicht lesen",
   "can't read inputfile",
   "akutes Eingabedateileseunvermoegen",
   "Sudden analphabetism: can't read inputfile"
                        },
  Ewriteoutput     [] = {
   "Kann Ausgabedatei nicht beschreiben",
   "can't write outputfile",
   "akute Ausgabedateischreiblaehmung",
   "Sudden dyslexia: can't write outputfile",
                        },
  Emissing_r       [] = {
   "Eingabedatei %s mit unerlaubtem Namen (kein 'r' am Ende) wird ignoriert\n",
   "inputfile %s with illegal name (no trailing 'r') is ignored\n",
   "Wie soll ich denn ein r von %s entfernen, wenn gar keins da ist ?\n",
   "How shall I drop a trailing 'r' from '%s', if there is none ??? Haeh?\n"
                         },
  Elevel0_ref      [] = {
   "Refinements ausserhalb einer Funktion gibts nicht",
   "refinements illegal outside functions",
   "Ein Refinement ?  Hier draussen in der Wueste ?",
   "Once upon a time, a little refinement stood alone in the functionless desert"
                        },
  Elines_in_func   [] = {
   "Funktion hat zuviele Zeilen",
   "too many lines in function",
   "Soooo viele Zeilen in einer Funktion haelt ja kein Computer aus",
   "Can't stand thaaat many lines in a single function"
                        },
  Ebytes_in_func   [] = {
   "Funktion zu lang (in Bytes)",
   "too many bytes in function",
   "Tolle Funktion.  Aber, in Bytes betrachtet, entschieden zu gross.",
   "Can't stand thaaat many bytes in a single function"
                        },
  Eref_not_decl    [] = {
   "Refinement nicht deklariert",
   "undeclared refinement",
   "Wer oder was soll das sein ?",
   "Hmm. I swear I don't know about this refinement"
                        },
  Erecursive_ref   [] = {
   "Rekursiver Refinementaufruf",
   "recursive call to refinement",
#define _rum_und_rum   "und rum und rum und rum und rum und rum und rum\
 und rum und rum (rekursives Refinement)",
   _rum_und_rum
   "recursive call to recursive call to recursive call to rec.... to refinement"
                        },
  Eunknown_leave   [] = {
   "zu verlassendes Refinement unbekannt",
   "refinement to leave is unknown",
   "WAS soll da verlassen werden ???",
   "WHAT do you want to leave ???"
                        },
  Eunterminated    [] = {
   "Schliessendes Anfuehrungszeichen fehlt",
   "Closing double quotes missing",
   "Huch, schon zu Ende ?  Wo sind die anderen Quotes ?",
   "(in a begging voice:) Please, give closing quotes for a poor string"
                        },
  Eleave_unpresent [] = {
#define _unpresent "Wir sind nicht in diesem Refinement, also kann es\
 auch nicht verlassen werden",
   _unpresent
   "you are not within this refinement, thus it's impossible to leave it",
#define _wenn_du   "Wenn Du denkst zu sein, wo ich denke (und du\
 schreibst), dass du zu sein denkst, denkst du falsch.",
   _wenn_du
  "You are not where you think you are. Thus you cannot leave from there."
                        },
  Eref_multi_decl  [] = {
   "Refinement mehrfach deklariert",
   "refinement declared more than once",
   "Langweilig!  Das Refinement gibts doch laengst",
   "Bah! Old stuff! I've long before known this refinement already",
                        },
  Ememory          [] = {
   "\n***** Speichermangel *****\n",
   "\n***** not enough memory *****\n",
   "\n  Des Bissele Speische is mi abe zu wenisch !\n",
   "\n***** memory claustrophobia *****\n",
                        },
  Eeof_brace       [] = {
   "Dateiende bei offener geschweifter Klammer",
   "EOF within block",
   "In der U-Bahn geboren, oder was ?   Mach erst mal alle Bloecke zu.",
   "Born in an underground train, or what ?  Close your blocks, please",
                        },
  Eeof_comment     [] = {
   "Dateiende im Kommentar",
   "EOF in comment",
#define _so_toll   "So toll sind Deine Kommentare nun auch nicht, dass\
 sie bis ans Dateiende gehen sollten",
   _so_toll
   "I do not think you should continue this comment all the rest of your life."
                        },
  Eeof_string      [] = {
   "Dateiende im Stringdenoter",
   "EOF in string denoter",
   "Hoerst Du immer mitten im Satz auf ?  Da ist noch ein String offen",
   "Oops! End of file, but no closing quotes for this string ?"
                        },
  Eeof_char        [] = {
   "Dateiende im Character-Denoter",
   "EOF in character denoter",
   "EOF in Character-Denoter.  Demnaechst halbieren sie noch die Bits....",
   "EOF in character denoter. It won't be long until they half the bits",
                        },
  Erefdecl_syntax  [] = {
   "falsche Syntax fuer Refinementdeklaration",
   "illegal syntax for refinement declaration",
   "Es begann wie eine Refinementdeklaration. Aber dann.....",
   "First it looked like a refinement declaration, but then..."
                        },
  Erefdecl_nested  [] = {
   "Mehr als ein Block offen bei Refinementdeklaration",
   "refinement declaration in nested block",
#define _du_bist "Du bist an dieser Stelle viel zu tief in dein Programm\
 verstrickt, um ein Refinement zu deklarieren",
   _du_bist
#define _unblock "A block in a block blocks refinement declarations.\
 Unblock the blocked refinement declarations by closing a block first"
  _unblock
                        },
  Emany_braces     [] = {
   "Zuviel schliessende Blockklammern",
   "too many closing braces",
#define _aufmachen   "Wenn Du jetzt noch eine Blockklammer aufmachst,\
 dann sind gar keine mehr offen",
   _aufmachen
   "If you open one more block now, none will be open at all..."
                        },
  Erefname_missing [] = {
   "Refinementsymbol ohne Refinementname",
   "refinement symbol found without refinement name",
   "Das Refinementsymbol fuehlt sich einsam so ganz ohne Name",
   "This refinement symbol will fell rather alone without a refinement name"
                        },
  Emany_errors     [] = {
   "Zu viele Fehler. Uebersetzung wird abgebrochen.",
   "too many errors,  compilation stopped",
   "Fiel zufiele Feeler",
   "I will now stop this horrorful compilation"
                        },
  Emany_warnings   [] = {
   "Zu viele Warnungen. Uebersetzung wird abgebrochen",
   "too many warnings, compilation stopped",
   "(So viele Warnungen sind mir nicht geheuer)",
   "Summing up, things look really a bit doubtful in this file."
                        };


