/*
 *  Embedded NNTP Client API
 *
 *  ./software/ch11/emnntp.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "nntpc.h"
#include "parsers.h"

#define MAX_LINE	200

static int sock=-1;
char buffer[MAX_LINE+1];

/* curMessage is the index of the next message index that can be retrieved
 * from the currently defined group.
 */
static int curMessage = -1;

/* firstMessage is the first available message index that can be retrieved
 * from the currently defined group.
 */
static int firstMessage = -1;

/* lastMessage is the last available message index that can be retrieved
 * from the currently defined group. 
 */
static int lastMessage=-1;


/*
 *  dialog()
 *
 *  Perform a dialog transaction with the connected NNTP server.  In this
 *  dialog, a command may be sent and a response may be returned.
 *
 *  The user passes in the character buffer containing an optional command
 *  (null string if no command is to be sent) and then the same string is
 *  used to collect the response.
 *
 *  The function returns 0 on success, -1 on error.
 *
 */

int dialog( int sd, char *buffer, char *resp, int rlen )
{
  int ret, len;

  if ((sd == -1) || (!buffer)) return -1;

  if (strlen(buffer) > 0) {
    len = strlen( buffer );
    if ( write( sd, buffer, len ) != len ) return -1;
  }

  if (resp != NULL) {
    ret = read( sd, buffer, MAX_LINE ); 
    if (ret >= 0) {
      buffer[ret] = 0;
      if (strncmp( buffer, resp, rlen )) return -1;
    } else {
      return -1;
    }
  }

  return 0;
}


/*
 *  nntpConnect()
 *
 *  Connect to an NNTP server using the server provided to this function.
 *
 *  Returns 0 on success, -1 on error.
 *
 */

int nntpConnect ( char *nntpServer )
{
  int result = -1;
  struct sockaddr_in servaddr;

  if (!nntpServer) return -1;

  sock = socket( AF_INET, SOCK_STREAM, 0 );

  bzero( &servaddr, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons( 119 );

  servaddr.sin_addr.s_addr = inet_addr( nntpServer );

  if ( servaddr.sin_addr.s_addr == 0xffffffff ) {
    struct hostent *hptr = (struct hostent *)gethostbyname( nntpServer );
    if ( hptr == NULL ) {
      return -1;
    } else {
      struct in_addr **addrs;
      addrs = (struct in_addr **)hptr->h_addr_list;
      memcpy( &servaddr.sin_addr, *addrs, sizeof(struct in_addr) );
    }
  }

  printf( "Connecting to %s\n", nntpServer );

  result = connect( sock, (struct sockaddr *)&servaddr, sizeof(servaddr) );

  if ( result >= 0 ) {

    buffer[0] = 0;
    result = dialog( sock, buffer, "201", 3 );

    if (result < 0) nntpDisconnect();

  }

  return ( result );
}


/*
 *  nntpSetGroup()
 *
 *  Makes the defined group the active group for the NNTP connection.  Also
 *  defines the last message read (through lastRead) to define which messages
 *  should be ignored.  If the lastRead argument is -1, then the first
 *  available message is defined as the next message to read.
 *
 *  Returns the number of messages available, or -1 on error.
 *
 */

int nntpSetGroup( char *group, int lastRead )
{
  int result = -1;
  int numMessages = -1;

  if ((!group) || (sock == -1)) return -1;

  snprintf( buffer, 80, "group %s\n", group );

  result = dialog( sock, buffer, "211", 3 );

  if (result == 0) {
    sscanf( buffer, "211 %d %d %d ", 
             &numMessages, &firstMessage, &lastMessage );

    if (lastRead == -1) {
      curMessage = firstMessage;
    } else {
      curMessage = lastRead+1;
      numMessages = lastMessage - lastRead;
    }

    printf("Set news group to %s\n", group);

  }

  return( numMessages );
}


/*
 *  nntpPeek()
 *
 *  This function peeks at the next message to be read, defined by the
 *  curMessage internal variable (defined by nntpSetGroup, not updated by
 *  this function).
 *
 *  The 'news' argument is filled with the current message headers, where 
 *  the user passes in the totalLen argument to define the maximum length
 *  of the message body with the 'news' structure.
 *
 *  Returns the size of the message read, or -1 on error.
 *
 */

int nntpPeek ( news_t *news, int totalLen )
{
  int result = -1, i, len=0, stop, state, bufIdx=0;

  if ((!news) || (sock == -1)) return -1;

  if ((curMessage == -1) || (curMessage > lastMessage)) return -2;

  /* Save the message id for this particular message */
  news->msgId = curMessage;

  snprintf( buffer, 80, "head %d\n", curMessage );

  result = dialog( sock, buffer, "221", 3 );

  if (result < 0) return -3;

  /* Skip the +OK response string and grab any data (end with CRLF) */
  len = strlen( buffer );
  for ( i = 0 ; i < len-1 ; i++ ) {
    if ( (buffer[i] == 0x0d) && (buffer[i+1] == 0x0a) ) {
      len -= i-2;
      memmove( news->msg, &buffer[i+2], len );
      bufIdx = len;
      break;
    }
  }

  state = stop = 0;
 
  while (!stop) {

    if (bufIdx+len > totalLen - 80) break;

    len = read( sock, &news->msg[bufIdx], (totalLen-bufIdx) );

    /* Search for the end-of-mail indicator in the current buffer */
    for ( i = bufIdx ; i < bufIdx+len ; i++ ) {
      if      ( (state == 0) && (news->msg[i] == 0x0d) ) state = 1;
      else if ( (state == 1) && (news->msg[i] == 0x0a) ) state = 2;
      else if ( (state == 2) && (news->msg[i] == 0x0d) ) state = 1;
      else if ( (state == 2) && (news->msg[i] ==  '.') ) state = 3;
      else if ( (state == 3) && (news->msg[i] == 0x0d) ) state = 4;
      else if ( (state == 4) && (news->msg[i] == 0x0a) ) { stop = 1; break; }
      else state = 0;
    }

    bufIdx += len;

  }

  bufIdx -= 3;
  news->msg[bufIdx] = 0;
  news->msgLen = bufIdx;

  return bufIdx;
}


/*
 *  nntpSkip()
 *
 *  This function skips the current message and sets the message index to
 *  the next message.
 *
 */

void nntpSkip( void )
{
  curMessage++;
}


/*
 *  nntpRetrieve()
 *
 *  This function retrieves the next message to be read, defined by the
 *  curMessage internal variable (defined by nntpSetGroup, updated by
 *  this function).
 *
 *  The 'news' argument is filled with the current message, where the
 *  user passes in the totalLen argument to define the maximum length
 *  of the message body with the 'news' structure.
 *
 *  Returns the size of the message read, or -1 on error.
 *
 */

int nntpRetrieve ( news_t *news, int totalLen )
{
  int result = -1, i, len=0, stop, state, bufIdx=0;

  if ((!news) || (sock == -1)) return -1;

  if ((curMessage == -1) || (curMessage > lastMessage)) return -1;

  /* Save the message id for this particular message */
  news->msgId = curMessage;

  snprintf( buffer, 80, "article %d\n", curMessage++ );

  result = dialog( sock, buffer, "220", 3 );

  if (result < 0) return -1;

  len = strlen(buffer);
  for ( i = 0 ; i < len-1 ; i++ ) {
    if ( (buffer[i] == 0x0d) && (buffer[i+1] == 0x0a) ) {
      len -= i-2;
      memmove( news->msg, &buffer[i+2], len );
      break;
    }
  }

  state = stop = 0;
 
  while (!stop) {

    if (bufIdx+len > totalLen - 80) break;

    /* Search for the end-of-mail indicator in the current buffer */
    for ( i = bufIdx ; i < bufIdx+len ; i++ ) {
      if      ( (state == 0) && (news->msg[i] == 0x0d) ) state = 1;
      else if ( (state == 1) && (news->msg[i] == 0x0a) ) state = 2;
      else if ( (state == 2) && (news->msg[i] == 0x0d) ) state = 1;
      else if ( (state == 2) && (news->msg[i] ==  '.') ) state = 3;
      else if ( (state == 3) && (news->msg[i] == 0x0d) ) state = 4;
      else if ( (state == 4) && (news->msg[i] == 0x0a) ) { stop = 1; break; }
      else state = 0;
    }

    bufIdx += (i-bufIdx);

    if (!stop) {

      len = read( sock, &news->msg[bufIdx], (totalLen-bufIdx) );

      if ( (len <= 0) || (bufIdx+len > totalLen) ) {
        break;
      }

    }

  }

  bufIdx -= 3;
  news->msg[bufIdx] = 0;
  news->msgLen = bufIdx;

  return bufIdx;
}


/*
 *  nntpParse()
 *
 *  This function parses the subject, sender and physical message from the
 *  news message (stored within the parameters in the 'news' structure).
 *
 *  Note that while the msg element of news contains the entire nntp message,
 *  the parsed message contains not the headers but instead only the 
 *  physical message.  The findBody routine identifies the physical message
 *  and initializes the 'bodyStart' element of the 'news' structure.
 *
 *  Returns 0 on success, or -1 on error.
 *
 */

int nntpParse( news_t *news, unsigned int flags )
{
  int result;

  if (!news) return -1;

  result = parseEntry( news, "Subject:", news->subject );
  if (result < 0) return -1;

  result = parseEntry( news, "Date:", news->msgDate );
  if (result < 0) return -2;

  result = parseEntry( news, "From:", news->sender );
  if (result < 0) return -3;
  fixAddress( news->sender );

  if (flags == FULL_PARSE) {
    result = findBody( news );
  }

  return result;
}


/*
 *  nntpDisconnect()
 *
 *  This function closes an active NNTP connection and initializes the
 *  internal state variables to 
 *
 *
 */

int nntpDisconnect ( void )
{
  if (sock == -1) return -1;
  close(sock);
  sock = curMessage = firstMessage = lastMessage = -1;
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
 *        its contributors may be used to endorse or promote products 
 *        derived from this software without specific prior 
 *        written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY CHARLES RIVER MEDIA AND CONTRIBUTERS 
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

