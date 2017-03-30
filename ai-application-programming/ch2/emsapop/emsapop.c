/*
 *  Population-based Simulated Annealing Implementation for n-Queens
 *
 *  ./software/ch2/emsapop/emsapop.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "common.h"

memberType population[MAX_MEMBERS];


/*
 *  initializePopulation()
 *
 *  Initialize the population of candidate solutions.
 *
 */

void initializePopulation( void )
{
  int member, index;
  int x, temp;

  for (member = 0 ; member < MAX_MEMBERS ; member++) {

    /* Initial setup of the solution */
    for (index = 0 ; index < MAX_LENGTH ; index++) {
      population[member].solution[index] = index+1;
    }

    /* Randomly perturb the solution */
    for (index = 0 ; index < MAX_LENGTH ; index++) {
      x = getRand(MAX_LENGTH);
      temp = population[member].solution[index];
      population[member].solution[index] = population[member].solution[x];
      population[member].solution[x] = temp;
    }

  }

}


/*
 *  tweakSolution()
 *
 *  Randomly perturb an encoded solution.
 *
 */

void tweakSolution( memberType *member )
{
  int temp, x, y;

  x = getRand(MAX_LENGTH);
  do {
    y = getRand(MAX_LENGTH);
  } while (x == y);

  temp = member->solution[x];
  member->solution[x] = member->solution[y];
  member->solution[y] = temp;
}


/*
 *  emitSolution()
 *
 *  Emit a solution in chessboard form.
 *
 */

void emitSolution( memberType *member )
{
  char board[MAX_LENGTH][MAX_LENGTH];
  int x, y;

  bzero( (void *)board, MAX_LENGTH * MAX_LENGTH );

  for (x = 0 ; x < MAX_LENGTH ; x++) {
    board[x][member->solution[x]-1] = 'Q';
  }

  printf("board:\n");
  for (y = 0 ; y < MAX_LENGTH ; y++) {
    for (x = 0 ; x < MAX_LENGTH ; x++) {
      if (board[x][y] == 'Q') printf("Q ");
      else printf(". ");
    }
    printf("\n");
  }
  printf("\n\n");
}


/*
 *  computeEnergy()
 *
 *  Calculate the energy of the passed solution.  The energy is the
 *  the number of conflicts on the board.  Note that only diagonals
 *  are checked.  The encoding ensures that no veritical or horizontal
 *  conflicts are possible.
 *
 */

float computeEnergy( memberType *member )
{
  int  i, j, x, y, tempx, tempy;
  char board[MAX_LENGTH][MAX_LENGTH];
  int conflicts;
  const int dx[4] = {-1,  1, -1,  1};
  const int dy[4] = {-1,  1,  1, -1};

  bzero( (void *)board, MAX_LENGTH * MAX_LENGTH );

  for (i = 0 ; i < MAX_LENGTH ; i++) {
    board[i][member->solution[i]-1] = 'Q';
  }

  /* Walk through each of the Queens, and compute the number of conflicts */
  conflicts = 0;

  for (i = 0 ; i < MAX_LENGTH ; i++) {

    x = i; y = member->solution[i]-1;

    /* NOTE: Based upon encoding, horizontal and vertical conflicts will 
     * never occur!!! 
     */

    /* Check diagonals */
    for (j = 0 ; j < 4 ; j++) {

      tempx = x ; tempy = y;
      while(1) {
        tempx += dx[j]; tempy += dy[j];
        if ((tempx < 0) || (tempx >= MAX_LENGTH) ||
               (tempy < 0) || (tempy >= MAX_LENGTH)) break;
        if (board[tempx][tempy] == 'Q') conflicts++;
      }

    }

  }

  return (float)conflicts;
}


/*
 *  simulatedAnnealing()
 *
 *  Perform the simulated annealing algorithm.
 *
 */

int simulateAnnealing( float curTemp )
{
  int member, i;
  memberType tempMember;
  float energy;
  int useNew = 0;
  int solution = -1;

  for (member = 0 ; member < MAX_MEMBERS ; member++) {

    for (i = 0 ; i < MAX_LENGTH ; i++) {
      tempMember.solution[i] = population[member].solution[i];
    }

    tweakSolution( &tempMember );

    energy = computeEnergy( &tempMember );

    useNew = 0;

    if (energy < population[member].energy) {
      useNew = 1;
    } else {
      float test = getSRand();
      float delta = energy - population[member].energy;

      if (exp(-delta/curTemp) > test) {
        useNew = 1;
      }
    }

    if (useNew) {

      for (i = 0 ; i < MAX_LENGTH ; i++) {
        population[member].solution[i] = tempMember.solution[i];
        population[member].energy = energy;
      }

    }

    if (population[member].energy == 0) solution = member;

  }

  return solution;
}


/* 
 *  computeAllEnergy()
 *
 *  Run through the population and compute the energy for each
 *  candidate solution.
 *
 */

void computeAllEnergy( void )
{
  int member;

  for (member = 0 ; member < MAX_MEMBERS ; member++) {
    population[member].energy = computeEnergy( &population[member] );
  }

}


/*
 *  main()
 *
 *  Main function for the simulated annealing demonstration.
 *
 */

int main()
{
  int  step, solution = -1;
  float temperature = INITIAL_TEMPERATURE;

  srand(time(NULL));

  initializePopulation();

  computeAllEnergy();

  while (temperature > FINAL_TEMPERATURE) {

    printf("temperature %f (solution %d)\n", temperature, solution);

    for (step = 0 ; step < STEPS_PER_CHANGE ; step++) {
      solution = simulateAnnealing( temperature );
    }

    temperature -= RATE;
  }

  if (solution == -1) {
    printf("No solution found\n");
  } else {
    emitSolution( &population[solution] );
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

