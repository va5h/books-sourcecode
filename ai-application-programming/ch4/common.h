/*
 *  Ant Algorotihms Symbolics, Types and Function Prototypes.
 *
 *  ./software/ch4/common.h
 *
 *  mtj@cogitollc.com
 *
 */

#ifndef __COMMON_H
#define __COMMON_H

#include <math.h>
#include <stdlib.h>

#define MAX_CITIES	15

#define MAX_DISTANCE	100

#define MAX_TOUR	(MAX_CITIES * MAX_DISTANCE)

typedef struct {
	int x;
	int y;
} cityType;


#define MAX_ANTS	20

typedef struct {
	int curCity;
	int nextCity;
	unsigned char tabu[MAX_CITIES];
	int pathIndex;
	unsigned char path[MAX_CITIES];
	double tourLength;
} antType;

#define getSRand()	((float)rand() / (float)RAND_MAX)
#define getRand(x)	(int)((x) * getSRand())

#define ALPHA		1.0
#define BETA		5.0
#define RHO		0.5	/* Intensity / Evaporation */
#define QVAL		100

#define MAX_TOURS	500

#define MAX_TIME	(MAX_TOURS * MAX_CITIES)

#define INIT_PHEROMONE	(1.0 / MAX_CITIES)

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

