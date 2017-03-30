/*
 *  Fuzzy Logic Simulated Battery Charge Controller
 *
 *  ./software/ch9/control.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include "fuzzy.h"
#include "mtemp.h"

int chargeMode = TRICKLE_CHARGE;


/*
 *  chargeControl()
 *
 *  Given temperature and voltage values, use the fuzzy logic 
 *  membership functions and fuzzy operators to determine the
 *  charge mode.
 *
 */

void chargeControl( float *timer )
{
  static unsigned int i = 0;

  extern float voltage, temperature;

  if ( (i++ % 10) == 0 ) {

    if (normalize( m_voltage_high( voltage ) ) ) {
      chargeMode = TRICKLE_CHARGE;
      *timer = 0.0;
    } else if (normalize( m_temp_hot( temperature ) ) ) {
      chargeMode = TRICKLE_CHARGE;
      *timer = 0.0;
    } else if (normalize( 
                   fuzzyAnd( fuzzyNot( m_voltage_high( voltage ) ),
                             fuzzyNot( m_temp_hot( temperature ) ) ) ) ) {
      chargeMode = FAST_CHARGE;
      *timer = 0.0;
    }

  }

  printf( "%d, %f, %f %d\n", i, voltage, temperature, chargeMode );
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

