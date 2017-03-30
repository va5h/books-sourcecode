/*
 *  Bigram demonstration
 *
 *  ./software/ch10/bigram.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_WORD_LEN	40
#define MAX_WORDS	1000

#define FIRST_WORD	0
#define MIDDLE_WORD	1
#define LAST_WORD	2

#define START_SYMBOL	0
#define END_SYMBOL	1

static int curWord = 2;

char wordVector[MAX_WORDS][MAX_WORD_LEN];

int  bigramArray[MAX_WORDS][MAX_WORDS];
int  sumVector[MAX_WORDS];

#define getSRand()	((float)rand() / (float)RAND_MAX)
#define getRand(x)	(int)((x) * getSRand())


// Bigram array uses two special symbols -- start and end.
// These symbols act just like words, but indicate probabilities of
// word beginning or ending.


/*
 *  loadWord()
 *
 *  This function loads a word into the wordVector (if new) and then
 *  updates the bigram array based upon the current word sequence.
 *
 */

void loadWord( char *word, int order )
{
  int wordIndex;
  static int lastIndex = START_SYMBOL;
  
  /* First, see if the word has already been recorded */
  for (wordIndex = 2 ; wordIndex < curWord ; wordIndex++) {
    if (!strcmp(wordVector[wordIndex], word)) {
      break;
    }
  }

  if (wordIndex == curWord) {

    if (curWord == MAX_WORDS) {
      printf("\nToo may words, increase MAX_WORDS\n\n");
      exit(-1);
    }

    /* Doesn't exist, add it in */
    strcpy(wordVector[curWord++], word);
  }

  /* At this point, we have a wordIndex that points to the current word
   * vector.
   */

  if (order == FIRST_WORD) {
    bigramArray[START_SYMBOL][wordIndex]++;
    sumVector[START_SYMBOL]++;
  } else if (order == LAST_WORD) {
    bigramArray[wordIndex][END_SYMBOL]++;
    bigramArray[END_SYMBOL][wordIndex]++;
    sumVector[END_SYMBOL]++;
  } else {
    bigramArray[lastIndex][wordIndex]++;
    sumVector[lastIndex]++;
  }
  lastIndex = wordIndex;

  return;
}


/*
 *  nextWord()
 *
 *  This function gets the next word from the bigramArray based
 *  probabilistically on the current word.
 *
 */

int nextWord( int word )
{
  int nextWord = (word + 1);
  int max = sumVector[word];
  int lim, sum = 0;

  lim = getRand(max)+1;

  while (nextWord != word) {

    nextWord = nextWord % curWord;

    sum += bigramArray[word][nextWord];

    if (sum >= lim) {
      return nextWord;
    }

    nextWord++;

  }

  return nextWord;
}


/*
 *  buildSentence()
 *
 *  This function creates a random (probabilistic) sentence from the
 *  data contained within the bigramArray.
 *
 */

int buildSentence( void )
{
  int word = START_SYMBOL;
  int max = 0;

  printf("\n");

  word = nextWord(word);

  while (word != END_SYMBOL) {

    printf("%s ", wordVector[word]);

    word = nextWord(word);

    max += getRand(12) + 1;

    if (max >= 100) break;

  }

  printf("%c.\n\n", 8);

  return 0;
}


/*
 *  parseOptions()
 *
 *  This function parses the command-line options and sets global
 *  flags accordingly.
 *
 */

void parseOptions( char *argv[], int argc, int *dbg, char *fname )
{
  int opt, error = 1;

  *dbg = 0;

  if (argc > 1) {

    while ((opt = getopt(argc, argv, "vf:")) != -1) {

      switch(opt) {
        
        case 'v':
          *dbg = 1;
          break;

        case 'f':
          strcpy(fname, optarg);
          error = 0;
          break;

        default:
          error = 1;

      }

    }

  }

  if (error) {
    printf("\nUsage is : \n\n");
    printf("\t%s -f <filename> -v\n\n", argv[0]);
    printf("\t\t -f corpus filename\n\t\t -v verbose mode\n\n");
    exit(0);
  }

  return;
}


/*
 *  parseFile()
 *
 *  This function parses the input file (corpus) and loads the words
 *  into the bigramArray as each is parsed.
 *
 */

void parseFile( char *filename )
{
  FILE *fp;
  int  inp, index = 0;
  char word[MAX_WORD_LEN+1];
  int  first = 1;

  fp = fopen(filename, "r");

  while (!feof(fp)) {

    inp = fgetc(fp);

    if (inp == EOF) {

      if (index > 0) {
        word[index++] = 0;
        loadWord(word, LAST_WORD);
        index = 0;
      }

    } else if (((char)inp == 0x0d) || ((char)inp == 0x0a) || 
               ((char)inp == ' ')) {

      if (index > 0) {
        word[index++] = 0;
        if (first) {
          loadWord(word, FIRST_WORD);
          index = 0;
          first = 0;
        } else {
          loadWord(word, MIDDLE_WORD);
          index = 0;
        }
      }

    } else if (((char)inp == '.') || ((char)inp == '?')) {

      word[index++] = 0;
      loadWord(word, MIDDLE_WORD);
      loadWord(word, LAST_WORD);
      index = 0;
      first = 1;

    } else {

      if (((char)inp != 0x0a) && ((char)inp != ',')) {
        word[index++] = (char)inp;
      }

    }

  }

  fclose(fp);
}


/*
 *  emitMatrix()
 *
 *  This test function is used to emit the bigramArray.
 *
 */

void emitMatrix( void )
{
  int x, y;

  printf("\n");
  for (x = 0 ; x < curWord ; x++) {
    printf("%20s : ", wordVector[x]);
    for ( y = 0 ; y < curWord ; y++) {
      printf("%d ", bigramArray[x][y]);
    }
    printf(" : %d\n", sumVector[x]);
  }
}


/*
 *  main()
 *
 *  This is the main function for the bigram demonstration.  It parses the
 *  options from the command line, initializes the bigramArray, parses the
 *  corpus file and then emits a random (probabilistic) sentence.
 *
 */

int main( int argc, char *argv[] )
{
  char filename[40];
  int  debug = 0;

  parseOptions( argv, argc, &debug, filename );

  srand(time(NULL));

  bzero(bigramArray, sizeof(bigramArray));

  strcpy(wordVector[0], "<START>");
  strcpy(wordVector[1], "<END>");

  parseFile( filename );

  if (debug) emitMatrix();

  printf("unique = %d\n", curWord);

  buildSentence();

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
 *        its contributors may be used to endorse or promote products 
 *        derived from this software without specific prior 
 *        written permission.
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

