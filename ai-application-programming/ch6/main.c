/*
 *  Genetic Algorithm main()
 *
 *  ./software/ch6/main.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "common.h"

extern void initPopulation(void);
extern POPULATION_TYPE populations[2][MAX_CHROMS];

/*
 *  main()
 *
 *  This is the main function for the genetic algorithm simulation.
 *
 */

int main()
{
  int generation = 0, i;
  FILE *fp;
  extern float minFitness, maxFitness, avgFitness;
  extern int curCrossovers, curMutations;
  extern int   curPop;

  void printProgram( int, int );

  srand(time(NULL));

  curPop = 0;

  fp = fopen("stats.txt", "w");

  if (fp == NULL) exit(-1);

  initPopulation();
  performFitnessCheck( fp );

  while (generation < MAX_GENERATIONS) {

    curCrossovers = curMutations = 0;

    performSelection();

    /* Switch the populations */
    curPop = (curPop == 0) ? 1 : 0;

    performFitnessCheck( fp );

    if ((generation++ % 100) == 0) {
      printf("Generation %d\n", generation-1);
      printf("\tmaxFitness = %f (%g)\n", maxFitness, MAX_FIT);
      printf("\tavgFitness = %f\n", avgFitness);
      printf("\tminFitness = %f\n", minFitness);
      printf("\tCrossovers = %d\n", curCrossovers);
      printf("\tMutation   = %d\n", curMutations);
      printf("\tpercentage = %f\n", avgFitness / maxFitness);
    }

    if ( generation > (MAX_GENERATIONS * 0.25) ) {
      if ((avgFitness / maxFitness) > 0.98) {
        printf("converged\n");
        break;
      }
    }

    if (maxFitness == MAX_FIT) {
      printf("found solution\n");
      break;
    }

  }

  printf("Generation %d\n", generation-1);
  printf("\tmaxFitness = %f (%g)\n", maxFitness, MAX_FIT);
  printf("\tavgFitness = %f\n", avgFitness);
  printf("\tminFitness = %f\n", minFitness);
  printf("\tCrossovers = %d\n", curCrossovers);
  printf("\tMutation   = %d\n", curMutations);
  printf("\tpercentage = %f\n", avgFitness / maxFitness);

  for (i = 0 ; i < MAX_CHROMS ; i++) {

    if (populations[curPop][i].fitness == maxFitness) {
      int index;
      printf("Program %3d : ", i);

      for (index = 0 ; index < populations[curPop][i].progSize ; index++) {
        printf("%02d ", populations[curPop][i].program[index]);
      }
      printf("\n");
      printf("Fitness %f\n", populations[curPop][i].fitness);
      printf("ProgSize %d\n\n", populations[curPop][i].progSize);

      printProgram(i, curPop);

      break;
    }

  }

  return 0;
}

/*
 *  printProgram
 *
 *  This function emits the program for a given chromosome in the defined
 *  population.  It is test code only and is not used in the simulation.
 *
 */

char *tins[]={  "DUP",
                "SWAP",
                "MUL",
                "ADD",
                "OVER",
                "NOP" };

void printProgram( int index, int curPop )
{
  int i;

  for (i = 0 ; i < populations[curPop][index].progSize ; i++) {
    printf("%s\n", tins[ populations[curPop][index].program[i] ] );
  }
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

