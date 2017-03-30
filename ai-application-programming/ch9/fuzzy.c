/*
 *  Fuzzy Logic Basic Operators
 *
 *  ./software/ch9/fuzzy.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <assert.h>
#include "fuzzy.h"

#define MAX(a,b)	((a>b) ? a : b)
#define MIN(a,b)	((a<b) ? a : b)

/*
 *  fuzzyAnd()
 *
 *  Fuzzy version of the AND operator.
 *
 */

fuzzyType fuzzyAnd( fuzzyType a, fuzzyType b )
{
  assert(a >= 0.0);
  assert(a <= 1.0);
  assert(b >= 0.0);
  assert(b <= 1.0);

  return MAX(a,b);
}


/*
 *  fuzzyOr()
 *
 *  Fuzzy version of the OR operator.
 *
 */

fuzzyType fuzzyOr( fuzzyType a, fuzzyType b )
{
  assert(a >= 0.0);
  assert(a <= 1.0);
  assert(b >= 0.0);
  assert(b <= 1.0);

  return MIN(a,b);
}


/*
 *  fuzzyNot()
 *
 *  Fuzzy version of the NOT operator.
 *
 */

fuzzyType fuzzyNot( fuzzyType a )
{
  assert(a >= 0.0);
  assert(a <= 1.0);

  return( 1.0 - a );
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

