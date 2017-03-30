/*
 *  Rules based system parser
 *
 *  ./software/ch8/parser.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"

#define MAX_FILE_SIZE	16384

extern memoryElementType workingMemory[MAX_MEMORY_ELEMENTS];
extern ruleType ruleSet[MAX_RULES];
extern int ruleIndex;


/*
 *  skipWhiteSpace()
 *
 *  Skip the white space (in the context of rules grammar).
 *
 */

char *skipWhiteSpace( char *block )
{
  char ch;

  while (1) {

    ch = *block;

    while ((ch != '(') && (ch != ')') && (ch != '=') && 
           (ch != 0) && (ch != ';')) {
      block++;
      ch = *block;
    }

    if (ch == ';') {

      while (*block++ != 0x0a);

    } else break;

  }

  return block;
}


/*
 *  parseElement()
 *
 *  Parse a single consequent or antecedent from the file.
 *
 */

char *parseElement( char *block, memoryElementType **met )
{
  memoryElementType *element;
  int i=0;
  int balance = 1;

  element = (memoryElementType *)malloc(sizeof(memoryElementType));

  element->element[i++] = *block++;

  while (1) {

    if (*block == 0) break;

    if (*block == ')') balance--;
    if (*block == '(') balance++;

    element->element[i++] = *block++;

    if (balance == 0) break;

  }

  element->element[i] = 0;
  element->next = 0;

  if (*met == 0) *met = element;
  else {
    memoryElementType *chain = *met;
    while (chain->next != 0) chain = chain->next;
    chain->next = element;
  }

  return block;
}


/*
 *  parseAntecedent()
 *
 *  Parse an antecedent from the file.
 *
 */

char *parseAntecedent( char *block, ruleType *rule )
{
  while (1) {

    block = skipWhiteSpace( block );

    if (*block == '(') {

      block = parseElement( block, &rule->antecedent );

    } else break;

  }

  return block;
}


/*
 *  parseConsequent()
 *
 *  Parse a consequent from the file.
 *
 */

char *parseConsequent( char *block, ruleType *rule )
{
  while (1) {

    block = skipWhiteSpace( block );

    if (*block == '(') {

      block = parseElement(block, &rule->consequent);

    } else break;

  }

  return block;
}


/*
 *  parseFile()
 *
 *  Parse the rules file.  This function parses the high level elements
 *  of the file, leaving parsing the antecedent and consequents to other
 *  functions.
 *
 */

int parseFile( char *filename )
{
  FILE *fp;
  char *file, *cur;
  int fail = 0;

  extern int debug;

  file = (char *)malloc(MAX_FILE_SIZE);

  if (file == NULL) return -1;

  fp = fopen(filename, "r");

  if (fp == NULL) {
    free(file);
    return -1;
  }

  fread( file, MAX_FILE_SIZE, 1, fp);

  cur = &file[0];

  while (1) {

    /* This will parse an entire rule */

    /* Find the "(defrule" start of a rule */
    cur = strstr( cur, "(defrule" );

    if (cur == NULL) {
      fail = 1;
      break;
    }

    if (!strncmp(cur, "(defrule", 8)) {
      int i=0;

      cur+=9;

      while (*cur != 0x0a) {
        ruleSet[ruleIndex].ruleName[i++] = *cur++;
      }
      ruleSet[ruleIndex].ruleName[i++] = 0;

      cur = skipWhiteSpace( cur );

      /* Parse the antecedents */
      cur = parseAntecedent( cur, &ruleSet[ruleIndex] );

      if (cur == NULL) {
        fail = 1;
        break;
      }

      /* Should be sitting on the '=>' */
      if (!strncmp(cur, "=>", 2)) {

        cur = skipWhiteSpace( cur+2 );

        /* Parse the consequents */
        cur = parseConsequent( cur, &ruleSet[ruleIndex] );

        if (cur == NULL) {
          fail = 1;
          break;
        }

        /* Ensure we're closing out the current rule */
        if (*cur == ')') {
          cur = skipWhiteSpace( cur+1 );
        } else {
          fail = 1;
          break;
        }

      } else {
        fail = 1;
        break;
      }

      ruleSet[ruleIndex].active = 1;
      ruleIndex++;

    } else {

      break;

    }

  }

  if (debug) {
    printf("Found %d rules\n", ruleIndex);
  }

  free( (void *)file );

  fclose(fp);

  return 0;
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

