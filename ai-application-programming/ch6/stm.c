/*
 *  Genetic Algorithm Stack Machine (Virtual Computer) Implementation
 *
 *  ./software/ch6/stm.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include "common.h"

#define STACK_DEPTH	25

int stack[STACK_DEPTH];
int stackPointer;

#define ASSERT_STACK_ELEMENTS(x) \
	 if (stackPointer < x) { error = STACK_VIOLATION ; break; }

#define ASSERT_STACK_NOT_FULL \
	 if (stackPointer == STACK_DEPTH) { error = STACK_VIOLATION ; break; }

#define SPUSH(x) (stack[stackPointer++] = x)
#define SPOP     (stack[--stackPointer])
#define SPEEK    (stack[stackPointer-1])


/*
 *  interpretSTM()
 *
 *  This function is the stack machine interpreter.  The program, its
 *  length, and 'argsLength' number of arguments are passed to perform
 *  whatever function is provided within 'program'.  Upon completion,
 *  any error encountered is returned to the caller.  The global stack
 *  'stack' is used to determine the result of the program and to test
 *  what was expected (which determines the fitness).
 *
 */

int interpretSTM(const int *program, int progLength, 
                   const int *args, int argsLength)
{
  int pc = 0;
  int i, error = NONE;
  int a, b;

  stackPointer = 0;

  /* Load the arguments onto the stack */
  for (i = argsLength-1 ; i >= 0 ; i--) {
    SPUSH(args[i]);
  }
  
  /* Execute the program */
  while ((error == NONE) && (pc < progLength)) {

    switch(program[pc++]) {

      case DUP:
        ASSERT_STACK_ELEMENTS(1);
        ASSERT_STACK_NOT_FULL;
        SPUSH(SPEEK);
        break;

      case SWAP:
        ASSERT_STACK_ELEMENTS(2);
        a = stack[stackPointer-1];
        stack[stackPointer-1] = stack[stackPointer-2];
        stack[stackPointer-2] = a;
        break;

      case MUL:
        ASSERT_STACK_ELEMENTS(2);
        a = SPOP; b = SPOP;
        SPUSH(a * b);
        break;

      case ADD:
        ASSERT_STACK_ELEMENTS(2);
        a = SPOP; b = SPOP;
        SPUSH(a + b);
        break;

      case OVER:
        ASSERT_STACK_ELEMENTS(2);
        SPUSH(stack[stackPointer-2]);
        break;

    } /* Switch opcode */

  } /* Loop */

  return(error);
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

