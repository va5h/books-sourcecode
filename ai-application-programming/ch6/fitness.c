/*
 *  Genetic Algorithm Fitness Evaluation Functions (user-defined)
 *
 *  ./software/ch6/fitness.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "common.h"

extern POPULATION_TYPE populations[2][MAX_CHROMS];
extern int curPop;

float maxFitness;
float avgFitness;
float minFitness;


/*
 * NOTE: This function is different for each problem to be solved... 
 */

extern int stackPointer;
extern int stack[];

static int x = 0;
float totFitness;

/*
 *  performFitnessCheck()
 *
 *  This is the fitness function used to score the potential solution.
 *  This function can be modifed to solve different problems.
 *
 */

int performFitnessCheck( FILE *outP )
{
  int chrom, result, i;
  int args[10], answer;

  maxFitness = 0.0;
  avgFitness = 0.0;
  minFitness = 1000.0;
  totFitness = 0.0;

  for ( chrom = 0 ; chrom < MAX_CHROMS ; chrom++ ) {

    populations[curPop][chrom].fitness = 0.0;

    for ( i = 0 ; i < COUNT ; i++ ) {

      args[0] = (rand() & 0x1f) + 1;
      args[1] = (rand() & 0x1f) + 1;
      args[2] = (rand() & 0x1f) + 1;

      /* Problem is xy + y^2 + z */
      answer = (args[0] * args[1]) + (args[1] * args[1]) + args[2];

      result = interpretSTM(populations[curPop][chrom].program, 
                            populations[curPop][chrom].progSize,
                            args, 3);

      if (result == NONE) {
        populations[curPop][chrom].fitness += TIER1;
      }

      if (stackPointer == 1) populations[curPop][chrom].fitness += TIER2;

      if (stack[0] == answer) populations[curPop][chrom].fitness += TIER3;

    }

    if (populations[curPop][chrom].fitness > maxFitness) {
      maxFitness = populations[curPop][chrom].fitness;
    } else if (populations[curPop][chrom].fitness < minFitness) {
      minFitness = populations[curPop][chrom].fitness;
    }

    totFitness += populations[curPop][chrom].fitness;

  }

  avgFitness = totFitness / (float)MAX_CHROMS;

  if (outP) {
    fprintf(outP, "%d %6.4f %6.4f %6.4f\n", 
             x++, minFitness, avgFitness, maxFitness);
  }

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

