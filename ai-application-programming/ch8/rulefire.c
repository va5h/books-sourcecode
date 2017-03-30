/*
 *  Rules based system actions
 *
 *  ./software/ch8/rulefire.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "common.h"

extern memoryElementType workingMemory[MAX_MEMORY_ELEMENTS];
extern ruleType ruleSet[MAX_RULES];


/*
 *  findEmptyMemSlot()
 *
 *  Find an empty slot in the working memory.
 *
 */

int findEmptyMemSlot( void )
{
  int i;

  for (i = 0 ; i < MAX_MEMORY_ELEMENTS ; i++) {
    if (!workingMemory[i].active) break;
  }

  return i;
}


/*
 *  constructElement()
 *
 *  Construct a fact from the current element, substituting the
 *  second argument if necessary.
 *
 */

void constructElement( char *new, const char *old, const char *arg )
{
  /* Find the second paren */
  old++;
  while (*old != '(') old++;

  while ((*old != 0) && (*old != '?')) *new++ = *old++;

  /* This was a complete rule (i.e., no ? element) */
  if (*old == 0) {

    *(--new) = 0;
    return;

  } else {

    /* Copy in the arg */
    while (*arg != 0) *new++ = *arg++;
    if ( *(new-1) != ')') *new++ = ')';
    *new = 0;

  }

  return;
}


/*
 *  performAddCommand()
 *
 *  Add a fact to the working memory.
 *
 */

int performAddCommand( char *mem )
{
  int slot;

  /* Check to ensure that this element isn't already in working memory */
  for (slot = 0 ; slot < MAX_MEMORY_ELEMENTS ; slot++) {

    if (workingMemory[slot].active) {

      if (!strcmp( workingMemory[slot].element, mem )) {

        /* Element is already here, return */
        return 0;

      }

    }

  }

  /* Add this element to working memory */

  slot = findEmptyMemSlot();

  if (slot < MAX_MEMORY_ELEMENTS) {

    workingMemory[slot].active = 1;
    strcpy( workingMemory[slot].element, mem );

  } else {
    assert(0);
  }

  return 1;
}


/*
 *  performDeleteCommand()
 *
 *  Delete a fact from the working memory.
 *
 */

int performDeleteCommand( char *mem )
{
  int slot;
  int ret = 0;
  char term1[MEMORY_ELEMENT_SIZE+1];
  char term2[MEMORY_ELEMENT_SIZE+1];
  char wm_term1[MEMORY_ELEMENT_SIZE+1];
  char wm_term2[MEMORY_ELEMENT_SIZE+1];

  sscanf( mem, "(%s %s)", term1, term2 );

  for ( slot = 0 ; slot < MAX_MEMORY_ELEMENTS ; slot++ ) {

    if ( workingMemory[slot].active ) {

      sscanf( workingMemory[slot].element, "(%s %s)", wm_term1, wm_term2 );

      if (!strncmp(term1, wm_term1, strlen(term1)) &&
          !strncmp(term2, wm_term2, strlen(term2))) {

        workingMemory[slot].active = 0;
        bzero( workingMemory[slot].element, MEMORY_ELEMENT_SIZE );
        ret = 1;

      }

    }

  }

  return ret;
}


/*
 *  performPrintCommand()
 *
 *  Print an arbitrary string.
 *
 */

int performPrintCommand( char *element )
{
  char string[MAX_MEMORY_ELEMENTS+1];
  int i=0, j=0;

  /* Find initial '"' */
  while ( element[i] != '"') i++;
  i++;

  /* Copy until we reach the end */
  while ( element[i] != '"') string[j++] = element[i++];
  string[j] = 0;

  printf("%s\n", string);

  return 1;
}


/*
 *  performEnableCommand()
 *
 *  Enable a timer, with a given expiration.
 *
 */

int performEnableCommand( char *element )
{
  char *string;
  int timer, expiration;

  void startTimer( int, int );

  string = strstr( element, "timer" );

  sscanf( string, "timer %d %d", &timer, &expiration );

  startTimer( timer, expiration );

  return 1;
}


/*
 *  fireRule()
 *
 *  In firing the rule, we perform all of the actions in the consequent
 *  section of the rule.
 *
 */

int fireRule( int rule, const char *arg )
{
  int ret;
  memoryElementType *walker = ruleSet[rule].consequent;
  char newCons[MAX_MEMORY_ELEMENTS+1];

  while (walker) {

    if (!strncmp(walker->element, "(add", 4)) {

      constructElement( newCons, walker->element, arg );

      ret = performAddCommand( newCons );

    } else if (!strncmp(walker->element, "(delete", 7)) {

      constructElement( newCons, walker->element, arg );

      ret = performDeleteCommand( newCons );

    } else if (!strncmp(walker->element, "(disable", 8)) {

      ruleSet[rule].active = 0;
      ret = 1;

    } else if (!strncmp(walker->element, "(print", 6)) {

      ret = performPrintCommand( walker->element );

    } else if (!strncmp(walker->element, "(enable", 7)) {

      ret = performEnableCommand( walker->element );

    } else if (!strncmp(walker->element, "(quit", 5)) {

      extern int endRun;

      endRun = 1;

    }

    walker = walker->next;

  }

  return ret;
}


/*
 *  Copyright (c) 2003 Charles River Media.  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or 
 *  without modification, is hereby granted without fee provided 
 *  that the following conditions are met:
 * 
 *    1.  Redistributions of source code must retain the above 
 *        copyright notice, this list of conditions and the 
 *        following disclaimer.
 *    2.  Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the 
 *        following disclaimer in the documentation and/or other 
 *        materials provided with the distribution.
 *    3.  Neither the name of Charles River Media nor the names of 
 *        its contributors may be used to endorse or promote 
 *        products derived from this software without specific 
 *        prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY CHARLES RIVER MEDIA AND CONTRIBUTORS 
 * 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CHARLES
 * RIVER MEDIA OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

