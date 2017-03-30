/*
 *  Common Symbolics and Prototypes
 *
 *  ./software/ch6/common.h
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdlib.h>

#ifndef COMMON_H
#define COMMON_H

/* Genetic Algorithm Parameters */

#define CROSSOVER_PROB		0.8
#define MUTATION_PROB		0.02
#define MAX_GENERATIONS		10000
#define COUNT			10

#define MAX_CHROMS		3000
#define MAX_PROGRAM		6


/* Function Macros and Prototypes */

#define getSRand()	((float)rand() / (float)RAND_MAX)
#define getRand(x)	(int)((x) * getSRand())

int interpretSTM( const int *, int, const int *, int );

int performFitnessCheck ( FILE * );

int performSelection( void );

typedef struct population {
  float fitness;
  int   progSize;
  int   program[MAX_PROGRAM];
} POPULATION_TYPE;

#define TIER1   1                           // 1
#define TIER2   ((TIER1+1) * COUNT)         // 10
#define TIER3   ((TIER1+TIER2+2) * COUNT)   // 65
#define MAX_FIT (float)((TIER3 * COUNT) + (TIER2 * COUNT) + (TIER1 * COUNT))


/* Virtual Computer Symbolics */

#define DUP			0x00
#define SWAP			0x01
#define MUL			0x02
#define ADD			0x03
#define OVER			0x04
#define NOP			0x05

#define MAX_INSTRUCTION		(NOP+1)

#define NONE			0
#define STACK_VIOLATION		1
#define MATH_VIOLATION		2

#endif /* _COMMON_H */


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

