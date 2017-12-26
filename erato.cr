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

#include <stdio.h>

#define MAX        10000
#define PRIME      0
#define NON_PRIME  1

static int sieve[MAX+1];

int main ()
{
  `initialize;
  `do sieve;
  `make output;
  return (0);
  
`initialize:
  int current;
  for (current = 2; current <= MAX; current++)
    sieve[current] = PRIME;
    
`do sieve:
  int current_prime = 1;
  for (;;) {
    `find next bigger prime;  /* perhaps STOP here */
    `delete all multiples of current_prime;
  }

`find next bigger prime:
  int current_candidate = current_prime + 1;
  while (sieve[current_candidate] == NON_PRIME)
    if (current_candidate == MAX)
      leave `do sieve;    /* leave two refinements at once */
    else
      current_candidate++;
  /* now current_candidate is a prime (or we leave `sieve) */
  current_prime = current_candidate;
  
`delete all multiples of current_prime:
  int current = `first multiple of current_prime;
  while (current <= MAX) {
    sieve[current] = NON_PRIME;
    current += current_prime;
  }
  
`first multiple of current_prime:
  2 * current_prime

`make output:
  int current;  /* different from 'current' above */
  printf ("The primes between 2 and %d are\n", MAX);
  for (current = 2; current <= MAX; current++)
    if (`current is prime)
      printf ("%5d ", current);

`current is prime:
  sieve[current] == PRIME

} /* end of main() */
