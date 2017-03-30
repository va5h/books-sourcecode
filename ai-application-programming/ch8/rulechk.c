/*
 *  Rules based system matcher
 *
 *  ./software/ch8/rulechk.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

extern memoryElementType workingMemory[MAX_MEMORY_ELEMENTS];
extern ruleType ruleSet[MAX_RULES];

memoryElementType *chain = NULL;


/*
 *  searchWorkingMemory()
 *
 *  Search the working memory for the two passed terms.
 *
 */

int searchWorkingMemory( char *term1, char *term2 )
{
  int ret = 0;
  int curMem = 0;
  char wm_term1[MEMORY_ELEMENT_SIZE+1];
  char wm_term2[MEMORY_ELEMENT_SIZE+1];

  while (1) {

    if (workingMemory[curMem].active) {

      /* extract the memory element */
      sscanf(workingMemory[curMem].element, "(%s %s)", wm_term1, wm_term2);
      if (wm_term2[strlen(wm_term2)-1] == ')') wm_term2[strlen(wm_term2)-1] = 0;

      if ((!strncmp(term1, wm_term1, strlen(term1))) &&
          (!strncmp(term2, wm_term2, strlen(term2)))) {

        ret = 1;
        break;

      }

    }

    curMem++;

    if (curMem == MAX_MEMORY_ELEMENTS) {
      break;
    }

  }

  return ret;
}


/*
 *  addToChain()
 *
 *  Create a chain second terms from working memory where the first terms 
 *  match, and the second term in the antecedent is the '?' symbol.
 *
 */

void addToChain( char *element )
{
  memoryElementType *walker, *newElement;;

  newElement = (memoryElementType *)malloc(sizeof(memoryElementType));

  strcpy( newElement->element, element );

  if (chain == NULL) {
    chain = newElement;
  } else {
    walker = chain;
    while (walker->next) walker = walker->next;
    walker->next = newElement;
  }

  newElement->next = NULL;
}


/*
 *  checkPattern()
 *
 *  Try to match all of the antecedents of a rule to facts in working
 *  memory.  This includes matching antecedent rules with '?' terms
 *  with facts from working memory.
 *
 */

int checkPattern( int rule, char *arg )
{
  int ret=0;
  char term1[MEMORY_ELEMENT_SIZE+1];
  char term2[MEMORY_ELEMENT_SIZE+1];
  memoryElementType *antecedent = ruleSet[rule].antecedent;

  /* Build a replacement string (based upon the '?' element) */
  while (antecedent) {

    sscanf( antecedent->element, "(%s %s)", term1, term2);
    if (term2[strlen(term2)-1] == ')') term2[strlen(term2)-1] = 0;

    /* If the antecedent element is variable, find the matches
     * in the working memory and store the matched terms.
     */
    if (term2[0] == '?') {
      int i;
      char wm_term1[MEMORY_ELEMENT_SIZE+1];
      char wm_term2[MEMORY_ELEMENT_SIZE+1];

      for (i = 0 ; i < MAX_MEMORY_ELEMENTS ; i++) {

        if (workingMemory[i].active) {

          sscanf( workingMemory[i].element, "(%s %s)", wm_term1, wm_term2 );
          if (wm_term2[strlen(wm_term2)-1] == ')') 
            wm_term2[strlen(wm_term2)-1] = 0; 

          if (!strncmp(term1, wm_term1, strlen(term1))) addToChain(wm_term2);
      
        }

      }

    }

    antecedent = antecedent->next;

  }

  /* Now that we have the replacement strings, walk through the rules trying
   * the replacement string when necessary.
   */

  do {

    memoryElementType *curRulePtr, *temp;

    curRulePtr = ruleSet[rule].antecedent;

    while (curRulePtr) {

      sscanf( curRulePtr->element, "(%s %s)", term1, term2 );
      if (term2[strlen(term2)-1] == ')') term2[strlen(term2)-1] = 0;

      if (!strncmp( term1, "true", strlen(term1))) {
        ret = 1;
        break;
      } else {
        if ((term2[0] == '?') && (chain)) strcpy(term2, chain->element);
      }

      ret = searchWorkingMemory( term1, term2 );

      if (!ret) break;

      curRulePtr = curRulePtr->next;

    }

    if (ret) {

      /* Cleanup the replacement string chain */
      while (chain) {
        temp = chain;
        chain = chain->next;
        free(temp);
      }

      strcpy(arg, term2);

    } else {

      if (chain) {
        temp = chain;
        chain = chain->next;
        free(temp);
      }

    }

  } while (chain);

  return ret;
}


/*
 *  checkRule()
 *
 *  Check to see if the rule will fire, based upon the facts in
 *  working memory.
 *
 */

int checkRule( int rule ) 
{
  int fire = 0;
  char arg[MEMORY_ELEMENT_SIZE]={0};

  extern int fireRule( int, char * );

  fire = checkPattern(rule, arg);

  if (fire == 1) {

    fire = fireRule( rule, arg );

  }

  return fire;
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

