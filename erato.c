#line 1 "erato.cr"
/*************************************************************************
Module  : C-Refine example program
Author  : Lutz Prechelt, Karlsruhe
Date    : 23.01.91
**************************************************************************
/*
    Copyright (C) 1988,90 by Lutz Prechelt, Karlsruhe

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

#define MAX        10000
#define PRIME      0
#define NON_PRIME  1

static int sieve[MAX+1];

int main ()
{
  {   /* initialize  (Level 1) */
#line 36 "erato.cr"
  int current;
  for (current = 2; current <= MAX; current++)
    sieve[current] = PRIME;
  }
  {   /* do_sieve  (Level 1) */
  int current_prime = 1;
  for (;;) {
    {   /* find_next_bigger_prime  (Level 2) */
#line 48 "erato.cr"
    int current_candidate = current_prime + 1;
    while (sieve[current_candidate] == NON_PRIME)
      if (current_candidate == MAX)
    goto do_sieve_1;
#line 51 "erato.cr"
        /* leave two refinements at once */
      else
        current_candidate++;
    /* now current_candidate is a prime (or we leave `sieve) */
    current_prime = current_candidate;
    }
#line 43 "erato.cr"
    /* perhaps STOP here */
    {   /* delete_all_multiples_of_current_prime  (Level 2) */
#line 58 "erato.cr"
    int current = (
#line 65 "erato.cr"
    2 * current_prime);
#line 59 "erato.cr"
    while (current <= MAX) {
      sieve[current] = NON_PRIME;
      current += current_prime;
    }
    }
#line 45 "erato.cr"
  }
do_sieve_1: ;
  }
  {   /* make_output  (Level 1) */
#line 68 "erato.cr"
  int current;  /* different from 'current' above */
  printf ("The primes between 2 and %d are\n", MAX);
  for (current = 2; current <= MAX; current++)
    if ((
#line 75 "erato.cr"
    sieve[current] == PRIME)
#line 71 "erato.cr"
    )
      printf ("%5d ", current);
  }
#line 33 "erato.cr"
  return (0);
#line 77 "erato.cr"
} /* end of main() */
