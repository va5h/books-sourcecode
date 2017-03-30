/*
 *  Rules based system timer routines
 *
 *  ./software/ch8/timers.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include "common.h"

timerType timers[MAX_TIMERS];


/*
 *  fireTimer()
 *
 *  Fire the defined timer.  This involves adding a '(timer-triggered x)'
 *  fact to the working memory, with x='expired timer'.
 *
 */

int fireTimer( int timerIndex )
{
  int ret;
  char element[MEMORY_ELEMENT_SIZE+1];

  extern int performAddCommand( char *mem );

  sprintf( element, "(timer-triggered %d)", timerIndex );

  ret = performAddCommand( element );

  timers[timerIndex].active = 0;

  return ret;
}


/*
 *  startTimer()
 *
 *  Activate a timer with a given expiration time.
 *
 */

void startTimer( int index, int expiration )
{
  timers[index].expiration = expiration;
  timers[index].active = 1;

  return;
}


/*
 *  processTimers()
 *
 *  Process the active timer list, upon finding an expired timer, 
 *  fire it.
 *
 */

void processTimers( void )
{
  int i;

  for (i = 0 ; i < MAX_TIMERS ; i++) {

    if (timers[i].active) {

      if (--timers[i].expiration == 0) {

        fireTimer( i );

      }

    }

  }

  return;
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

