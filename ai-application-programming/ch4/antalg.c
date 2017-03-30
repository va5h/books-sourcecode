/*
 *  Ant Algorithm Demonstration
 *
 *  ./software/ch4/antalg.c
 *
 *  mtj@cogitollc.com
 *
 */


#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "common.h"

cityType cities[MAX_CITIES];

antType ants[MAX_ANTS];

              /*   From          To     */
double   distance[MAX_CITIES][MAX_CITIES];

              /*    From         To      */
double pheromone[MAX_CITIES][MAX_CITIES];

double  best=(double)MAX_TOUR;
int     bestIndex;


/*
 *  init()
 *
 *  Initialize the cities, their distances and the Ant population.
 *
 */

void init( void )
{
  int from, to, ant;

  /* Create the cities and their locations */
  for (from = 0 ; from < MAX_CITIES ; from++) {

    /* Randomly place cities around the grid */
    cities[from].x = getRand( MAX_DISTANCE );
    cities[from].y = getRand( MAX_DISTANCE );

    for (to = 0 ; to < MAX_CITIES ; to++) {
      distance[from][to] = 0.0;
      pheromone[from][to] = INIT_PHEROMONE;
    }

  }

  /* Compute the distances for each of the cities on the map */
  for ( from = 0 ; from < MAX_CITIES ; from++) {

    for ( to = 0 ; to < MAX_CITIES ; to++) {

      if ((to != from) && (distance[from][to] == 0.0)) {
        int xd = abs(cities[from].x - cities[to].x);
        int yd = abs(cities[from].y - cities[to].y);

        distance[from][to] = sqrt( (xd * xd) + (yd * yd) );
        distance[to][from] = distance[from][to];
      }

    }

  }

  /* Initialize the ants */
  to = 0;
  for ( ant = 0 ; ant < MAX_ANTS ; ant++ ) {

    /* Distribute the ants to each of the cities uniformly */
    if (to == MAX_CITIES) to = 0;
    ants[ant].curCity = to++;

    for ( from = 0 ; from < MAX_CITIES ; from++ ) {
      ants[ant].tabu[from] = 0;
      ants[ant].path[from] = -1;
    }

    ants[ant].pathIndex = 1;
    ants[ant].path[0] = ants[ant].curCity;
    ants[ant].nextCity = -1;
    ants[ant].tourLength = 0.0;

    /* Load the ant's current city into taboo */
    ants[ant].tabu[ants[ant].curCity] = 1;

  }
}


/*
 *  restartAnts()
 *
 *  Reinitialize the ant population to start another tour around the
 *  graph.
 *
 */

void restartAnts( void )
{
  int ant, i, to=0;

  for ( ant = 0 ; ant < MAX_ANTS ; ant++ ) {

    if (ants[ant].tourLength < best) {
      best = ants[ant].tourLength;
      bestIndex = ant;
    }

    ants[ant].nextCity = -1;
    ants[ant].tourLength = 0.0;

    for (i = 0 ; i < MAX_CITIES ; i++) {
      ants[ant].tabu[i] = 0;
      ants[ant].path[i] = -1;
    }

    if (to == MAX_CITIES) to = 0;
    ants[ant].curCity = to++;

    ants[ant].pathIndex = 1;
    ants[ant].path[0] = ants[ant].curCity;

    ants[ant].tabu[ants[ant].curCity] = 1;

  }

}


/*
 *  antProduct()
 *
 *  Compute the denominator for the path probability equation (concentration
 *  of pheromone of the current path over the sum of all concentrations of
 *  available paths).
 *
 */

double antProduct( int from, int to )
{
  return (( pow( pheromone[from][to], ALPHA ) *
            pow( (1.0 / distance[from][to]), BETA ) ));
}


/*
 *  selectNextCity()
 *
 *  Using the path probability selection algorithm and the current pheromone
 *  levels of the graph, select the next city the ant will travel to.
 *
 */

int selectNextCity( int ant )
{
  int from, to;
  double denom=0.0;

  /* Choose the next city to visit */
  from = ants[ant].curCity;

  /* Compute denom */
  for (to = 0 ; to < MAX_CITIES ; to++) {
    if (ants[ant].tabu[to] == 0) {
      denom += antProduct( from, to );
    }
  }

  assert(denom != 0.0);

  do {

    double p;

    to++;
    if (to >= MAX_CITIES) to = 0;

    if ( ants[ant].tabu[to] == 0 ) {

      p = antProduct(from, to)/denom;

      if (getSRand() < p ) break;

    }

  } while (1);

  return to;
}


/*
 *  simulateAnts()
 *
 *  Simulate a single step for each ant in the population.  This function
 *  will return zero once all ants have completed their tours.
 *
 */

int simulateAnts( void )
{
  int k;
  int moving = 0;

  for (k = 0 ; k < MAX_ANTS ; k++) {

    /* Ensure this ant still has cities to visit */
    if (ants[k].pathIndex < MAX_CITIES) {

      ants[k].nextCity = selectNextCity( k );

      ants[k].tabu[ants[k].nextCity] = 1;

      ants[k].path[ants[k].pathIndex++] = ants[k].nextCity;

      ants[k].tourLength += distance[ants[k].curCity][ants[k].nextCity];

      /* Handle the final case (last city to first) */
      if (ants[k].pathIndex == MAX_CITIES) {
        ants[k].tourLength += 
          distance[ants[k].path[MAX_CITIES-1]][ants[k].path[0]];
      }

      ants[k].curCity = ants[k].nextCity;

      moving++;

    }

  }

  return moving;
}


/*
 *  updateTrails()
 *
 *  Update the pheromone levels on each arc based upon the number of ants
 *  that have travelled over it, including evaporation of existing pheromone.
 *
 */

void updateTrails( void )
{
  int from, to, i, ant;

  /* Pheromone Evaporation */
  for (from = 0 ; from < MAX_CITIES ; from++) {

    for (to = 0 ; to < MAX_CITIES ; to++) {

      if (from != to) {

        pheromone[from][to] *= (1.0 - RHO);

        if (pheromone[from][to] < 0.0) pheromone[from][to] = INIT_PHEROMONE;

      }

    }

  }

  /* Add new pheromone to the trails */

  /* Look at the tours of each ant */
  for (ant = 0 ; ant < MAX_ANTS ; ant++) {

    /* Update each leg of the tour given the tour length */
    for (i = 0 ; i < MAX_CITIES ; i++) {

      if (i < MAX_CITIES-1) {
        from = ants[ant].path[i];
        to = ants[ant].path[i+1];
      } else {
        from = ants[ant].path[i];
        to = ants[ant].path[0];
      }

      pheromone[from][to] += (QVAL / ants[ant].tourLength);
      pheromone[to][from] = pheromone[from][to];

    }

  }

  for (from = 0 ; from < MAX_CITIES ; from++) {
    for (to = 0 ; to < MAX_CITIES ; to++) {
      pheromone[from][to] *= RHO;
    }
  }

}


/*
 *  emitDataFile()
 *
 *  For the ant with the best tour (shortest tour through the graph), emit
 *  the path in two data files (plotted together).
 *
 */

void emitDataFile( int ant )
{
  int city;
  FILE *fp;

  fp = fopen("cities.dat", "w");
  for (city = 0 ; city < MAX_CITIES ; city++) {
    fprintf(fp, "%d %d\n", cities[city].x, cities[city].y);
  }
  fclose(fp);

  fp = fopen("solution.dat", "w");
  for (city = 0 ; city < MAX_CITIES ; city++) {
    fprintf(fp, "%d %d\n", 
             cities[ ants[ant].path[city] ].x,
             cities[ ants[ant].path[city] ].y );
  }  
  fprintf(fp, "%d %d\n", 
           cities[ ants[ant].path[0] ].x,
           cities[ ants[ant].path[0] ].y );

  fclose(fp);
}



void emitTable( void )
{
  int from, to;

  for (from = 0 ; from < MAX_CITIES ; from++) {
    for (to = 0 ; to < MAX_CITIES ; to++) {
      printf("%5.2g ", pheromone[from][to]);
    }
    printf("\n");
  }
  printf("\n");
}


/*
 *  main()
 *
 *  Main function for the ant algorithm.  Performs the simulation given the
 *  constraints defined in common.h.
 *
 */

int main()
{
  int curTime = 0;

  srand( time(NULL) );

  init();

  while (curTime++ < MAX_TIME) {

    if ( simulateAnts() == 0 ) {

      updateTrails();

      if (curTime != MAX_TIME)
        restartAnts();

      printf("Time is %d (%g)\n", curTime, best);

    }

  }

  printf("best tour %g\n", best);
  printf("\n\n");

  emitDataFile( bestIndex );

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

