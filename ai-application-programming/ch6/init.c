/* 
 *  Genetic Algorithm Population Initialization
 *
 *  ./software/ch6/init.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "common.h"

POPULATION_TYPE populations[2][MAX_CHROMS];

int curPop;

/*
 *  initMember()
 *
 *  Initialize a single member in the population.  This includes the
 *  initial fitness, program size and the initial set of random
 *  instructions.
 *
 */

void initMember( pop, index )
{
  int progIndex;

  populations[pop][index].fitness = 0.0;
  populations[pop][index].progSize = MAX_PROGRAM-1;

  progIndex = 0;
  while (progIndex < MAX_PROGRAM) {
    populations[pop][index].program[progIndex++] = getRand(MAX_INSTRUCTION);
  }

}


/*
 *  initPopulation()
 *
 *  Initialize all of the chromosomes in the population (potential 
 *  solutions to the given problem).
 *
 */

void initPopulation( void )
{
  int index;

  for (index = 0 ; index < MAX_CHROMS ; index++) {
    initMember(curPop, index);
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

