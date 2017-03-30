/*
 *  Artificial Life Simulation Types and Symbolic Constants
 *
 *  ./software/ch7/common.h
 *
 *  mtj@cogitollc.com
 *
 */

#ifndef __COMMON_H
#define __COMMON_H

#include <math.h>
#include <stdlib.h>

/* Sensor Input Cells */

#define HERB_FRONT	0
#define CARN_FRONT	1
#define PLANT_FRONT	2
#define HERB_LEFT	3
#define CARN_LEFT	4
#define PLANT_LEFT	5
#define HERB_RIGHT	6
#define CARN_RIGHT	7
#define PLANT_RIGHT	8
#define HERB_PROXIMITY	9
#define CARN_PROXIMITY	10
#define PLANT_PROXIMITY	11

#define MAX_INPUTS	12


/* Output Cells */
 
#define ACTION_TURN_LEFT	0
#define ACTION_TURN_RIGHT	1
#define ACTION_MOVE		2
#define ACTION_EAT		3

#define MAX_OUTPUTS	4


/* Total number of weights (and biases) for an agent */

#define TOTAL_WEIGHTS	((MAX_INPUTS * MAX_OUTPUTS) + MAX_OUTPUTS)


/* Description of the 3 planes for the 2d grid */

#define HERB_PLANE	0
#define CARN_PLANE	1
#define PLANT_PLANE	2


/* Available directions */

#define NORTH	0
#define SOUTH	1
#define EAST	2
#define WEST	3

#define MAX_DIRECTION	4


/* Types for location, plants and agents */

typedef struct {
  short x;
  short y;
} locType;

typedef struct {
  locType location;
} plantType;

typedef struct {
  short type;
  short energy;
  short parent;
  short age;
  short generation;
  locType location;
  unsigned short direction;
  short inputs[MAX_INPUTS];
  short weight_oi[MAX_INPUTS * MAX_OUTPUTS];
  short biaso[MAX_OUTPUTS];
  short actions[MAX_OUTPUTS];
} agentType;

#define TYPE_HERBIVORE	0
#define TYPE_CARNIVORE	1
#define TYPE_DEAD	-1

typedef struct {
  short y_offset;
  short x_offset;
} offsetPairType;


/* Grid offsets for Front/Left/Right/Proximity (North/-South facing) */

const offsetPairType northFront[]=
      {{-2,-2}, {-2,-1}, {-2,0}, {-2,1}, {-2,2}, {9,9}};
const offsetPairType northLeft[]={{0,-2}, {-1,-2}, {9,9}};
const offsetPairType northRight[]={{0,2}, {-1,2}, {9,9}};
const offsetPairType northProx[]=
      {{0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {9,9}};

/* Grid offsets for Front/Left/Right/Proximity (West/-East facing) */

const offsetPairType westFront[]=
      {{2,-2}, {1,-2}, {0,-2}, {-1,-2}, {-2,-2}, {9,9}};
const offsetPairType westLeft[]={{2,0}, {2,-1}, {9,9}};
const offsetPairType westRight[]={{-2,0}, {-2,-1}, {9,9}};
const offsetPairType westProx[]=
      {{1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {9,9}};


/* Macro Function Definitions */

#define getSRand()	((float)rand() / (float)RAND_MAX)
#define getRand(x)	(int)((x) * getSRand())

#define getWeight()	(getRand(9)-1)


/* Parameters that can be adjusted */

#define MAX_FOOD_ENERGY		15
#define MAX_ENERGY		60
#define REPRODUCE_ENERGY	0.9

#define MAX_AGENTS	36

#define MAX_PLANTS	35

#define MAX_GRID	30

#define MAX_STEPS	1000000

#endif /* __COMMON_H */


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

