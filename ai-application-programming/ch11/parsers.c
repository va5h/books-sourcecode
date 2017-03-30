/*
 *  Embedded NNTP Message Parsers
 *
 *  ./software/ch11/emnntp/parsers.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <string.h>
#include <ctype.h>
#include "nntpc.h"

/*
 *  parseEntry()
 *
 *  The parseEntry function takes a 'news' structure and searches for
 *  the 'searchString' in the message headers.  The headers have the
 *  form 'XXX: YYY' where XXX is the keyword and YYY is the value for 
 *  the keyword.  When the keyword is found, the value is stored in
 *  'dest'.
 *
 *  This function returns 0 on success, -1 on error.
 *
 */
int parseEntry( news_t *news, const char *searchString, char *dest )
{
  int i, index, len;

  if ((!news) || (!dest) || (!searchString)) return -1;

  len = strlen(searchString);

  /* Grab the value for the searchString */
  for (i = 0 ; i < news->msgLen ; i++) {
    if (news->msg[i] == searchString[0]) {
      if (!strncmp(&news->msg[i], searchString, len )) {
        i += (len + 1);
        index = 0;
        while (!((news->msg[i] == 0x0d) && (news->msg[i+1] == 0x0a))) {
          dest[index++] = tolower(news->msg[i++]);
        }
        dest[index] = 0;
        break;
      }
    }
  }

  if (i == news->msgLen) {
    return -1;
  }

  return 0;
}


/*
 *  fixAddress()
 *
 *  The fixAddress function takes an email address in the form:
 *
 *    <xxx@yyy.zzz> and transforms it to xxx@yyy.zzz
 *
 *  The function returns -1 on error, 0 on success.
 *
 */

int fixAddress ( char *address )
{
  int i, j, len;
  char string[MAX_SM_STRING+1];

  if (!address) return -1;

  len = strlen(address);
  for (i = 0 ; i < len ; i++) {
    if (address[i] == '<') break;
  }

  if (i++ == len) return 0;

  j = 0;
  for ( ; i < len ; i++) {
    if (address[i] == '>') break;
    string[j++] = address[i];
  }
  string[j] = 0;

  strcpy(address, string);

  return 0;
}


/*
 *  findBody()
 *
 *  The findBody function finds the body of the message within the entire
 *  'news' message.  The body in this case excludes all headers and begins
 *  after the blank line after the news headers.
 *
 *  Returns 0 on success, -1 on error.
 *
 */

int findBody( news_t *news )
{
  int i, result=-1;

  for (i = 0 ; i < news->msgLen ; i++) {
    if ((news->msg[i] == 0x0d) && (news->msg[i+1] == 0x0a) &&
        (news->msg[i+2] == 0x0d) && (news->msg[i+3] == 0x0a)) {
      i+=4;
      break;
    }
  }

  if (i < news->msgLen) {
    news->bodyStart = &news->msg[i];
    result = 0;
  }

  return result;
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

