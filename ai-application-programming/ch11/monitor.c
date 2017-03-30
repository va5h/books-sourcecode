/*
 *  Simple HTTP Client
 *
 *  ./software/ch11/monitor.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "prsconf.h"

#define MAX_BUFFER		2048


/*
 *  prune()
 *
 *  The purpose of the prune function is to prune a URL down to
 *  a simple fully qualified domain name.
 *
 */
 
void prune( char *url, char *fqdn )
{
  int i;

  /* Walk through the array, copying from url to fqdn until a '/' is found. */
  for (i = 0 ; i < strlen(url) ; i++) {
    if (url[i+7] == '/') break;
    fqdn[i] = url[i+7];
  }
  fqdn[i] = 0;

  return;
}


/*
 *  grabResponse()
 *
 *  This function grabs an HTTP response, looking for the double
 *  newline (end of HTTP header).  
 *
 */

int grabResponse( int sock, char *buf )
{
  int i, len, stop, state, bufIdx;

  if (buf == NULL) return -1;

  len = bufIdx = state = stop = 0;
 
  /* Work through the state machine until the end of buffer, or end of the
   * HTTP message is found.
   */
  while (!stop) {

    /* If we're at the end, break */
    if (bufIdx+len > MAX_BUFFER - 80) break;

    /* Read the next set of data through the socket (server connection). */
    len = read( sock, &buf[bufIdx], (MAX_BUFFER-bufIdx) );

    /* Search for the end-of-mail indicator in the current buffer */
    for ( i = bufIdx ; i < bufIdx+len ; i++ ) {
      if      ( (state == 0) && (buf[i] == 0x0d) ) state = 1;
      else if ( (state == 1) && (buf[i] == 0x0a) ) state = 2;
      else if ( (state == 2) && (buf[i] == 0x0d) ) state = 3;
      else if ( (state == 3) && (buf[i] == 0x0a) ) { stop = 1; break; }
      else state = 0;
    }

    /* Increase the buffer by the last read amount */
    bufIdx += len;

  }

  /* Terminate the buffer */
  bufIdx -= 3;
  buf[bufIdx] = 0;

  /* Return the size of the buffer (characters in buf) */
  return bufIdx;
}



/*
 *  monitorSite()
 *
 *  This function tries to determine if a site has changed.  It acts
 *  as a simple HTTP client and reads the HTTP response header.  It
 *  uses the content length (length of the actual HTTP body) to identify
 *  whether the web-page has changed.
 *
 */

int monitorSite( int siteIndex )
{
  int ret=0, sock, result, len;
  struct sockaddr_in servaddr;
  char buffer[MAX_BUFFER+1];
  char fqdn[80];

  extern monitorEntryType monitors[];

  /* Create a new client socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);

  /* Prune the extraneous characters from the fqdn */
  prune( monitors[siteIndex].url, fqdn );

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons( 80 );

  /* Try and resolve the address */
  servaddr.sin_addr.s_addr = inet_addr( fqdn );

  /* If the target was not an IP address, then it must be a fully
   * qualified domain name.  Try to use the DNS resolver to resolve
   * the name to an address.
   */
  if ( servaddr.sin_addr.s_addr == 0xffffffff ) {
    struct hostent *hptr = 
      (struct hostent *)gethostbyname( fqdn );
    if ( hptr == NULL ) {
      close(sock);
      return -1;
    } else {
      struct in_addr **addrs;
      addrs = (struct in_addr **)hptr->h_addr_list;
      memcpy( &servaddr.sin_addr, *addrs, sizeof(struct in_addr) );
    }
  }

  /* Connect to the defined HTTP server */
  result = connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));

  if (result == 0) {

    /* Perform a simple HTTP GET command */
    strcpy(buffer, "GET / HTTP/1.0\n\n");

    /* Write our command to the web server */
    len = write(sock, buffer, strlen(buffer) );

    /* If we were able to write our command, continue */
    if ( len == strlen(buffer) ) {
      char *cur;

      /* Await the response from the web server */
      len = grabResponse( sock, buffer );

      /* Search the response for the content-length string */
      cur = strstr(buffer, "Content-Length:");

      if (cur != NULL) {
        int curLen;

        /* Parse the actual length from the response */
        sscanf(buffer, "Content-Length: %d", &curLen);

        /* If the length of the remote content has changed, update the
         * structure accordingly.
         */
        if (len != monitors[siteIndex].length) {
          monitors[siteIndex].shown = 0;
          monitors[siteIndex].changed = 1;
          monitors[siteIndex].length = len;
          ret = 1;
        }

      }

    }

  }

  close(sock);

  return(ret);
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

