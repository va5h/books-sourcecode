/*
 *  Simple HTTP Server
 *
 *  ./software/ch11/https.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "protos.h"


static int listenfd=-1;

#define MAX_BUFFER 4096

const char *notfound={"HTTP/1.1 404 File not found\n\n"};


/*
 *  emitHTTPResponseHeader()
 *
 *  Emit a simple HTTP response header for HTML.
 *
 */

void emitHTTPResponseHeader( int connfd )
{
  char line[80];

  strcpy( line, "HTTP/1.1 200 OK\n" );
  write( connfd, line, strlen(line) );

  strcpy( line, "Server: tinyHttp\n" );
  write( connfd, line, strlen(line) );

  strcpy( line, "Connection: close\n" );
  write( connfd, line, strlen(line) );

  strcpy( line, "Content-Type: text/html\n\n" );
  write( connfd, line, strlen(line) );

  return;
}


/*
 *  getFilename()
 *
 *  This function parses the filename out of an HTTP GET/POST request.
 *  If no filename is specified ("/"), then the filename is automatically
 *  defined as "/index.html".
 *
 */

void getFilename(char *inbuf, char *out, int start)
{
  int i=start, j=0;

  /* Skip any initial spaces */
  while (inbuf[i] == ' ') i++;

  /* Grab the filename and store in 'out' */
  for ( ; i < strlen(inbuf) ; i++) {
    if (inbuf[i] == ' ') {
      out[j] = 0;
      break;
    }
    out[j++] = inbuf[i];
  }

  /* If the default '/' is found, translate this into '/index.html'. */
  if (!strcmp(out, "/")) strcpy(out, "/index.html");

  return;
}


/*
 *  handleConnection()
 *
 *  This function handles an HTTP connection.  Upon parsing the HTTP
 *  request message, the request type is parsed and then the filename.
 *  The particular handler function is then called to satisfy the client
 *  request.
 *
 */

void handleConnection( int connfd )
{
  int len, max, loop;
  char buffer[MAX_BUFFER+1];
  char filename[80+1];

  /* Read in the HTTP request message */
  max = 0; loop = 1;
  while (loop) {
    len = read(connfd, &buffer[max], 255); buffer[max+len] = 0;
    if (len <= 0) return;
    max += len;
    if ((buffer[max-4] == 0x0d) && (buffer[max-3] == 0x0a) &&
        (buffer[max-2] == 0x0d) && (buffer[max-1] == 0x0a)) loop = 0;
  }

  /* Determine the HTTP request */
  if (!strncmp(buffer, "GET", 3)) {

    getFilename(buffer, filename, 4);

    /* Within this tiny HTTP server, the filename parsed from the
     * request determines the function to call to emit an HTTP
     * response.
     */
    if      (!strncmp(filename, "/index.html", 11)) emitNews( connfd );
    else if (!strncmp(filename, "/config.html", 12)) emitConfig( connfd );
    else if (!strncmp(filename, "/art", 3)) emitArticle( connfd, filename );
    else write(connfd, notfound, strlen(notfound));

  } else if (!strncmp(buffer, "POST", 4)) {

    getFilename(buffer, filename, 5);

    /* Ditto for the POST request (as above for GET).  The POST filename
     * determines what to do -- the only case though is the "Mark Read"
     * button which clears the shown entries.
     */
    if (!strncmp(filename, "/clear", 6)) {
      clearEntries();
      emitHTTPResponseHeader( connfd );
      strcpy(buffer, "<P><H1>Click Back and Reload to "
                     "refresh page.</H1><P>\n\n");
      write(connfd, buffer, strlen(buffer));
    } else {
      write(connfd, notfound, strlen(notfound));
    }

  } else {

    /* Did not recognize the request -- return a not-implemented status */
    strcpy(buffer, "HTTP/1.1 501 Not Implemented\n\n");
    write(connfd, buffer, strlen(buffer));

  }

  return;
}


/*
 *  initHttpServer()
 *
 *  This function creates the HTTP server socket.
 *
 */

int initHttpServer( void )
{
  int on=1, ret;
  struct sockaddr_in servaddr;

  if (listenfd != -1) close( listenfd );

  listenfd = socket( AF_INET, SOCK_STREAM, 0 );

  /* Make the port immediately reusable */
  ret = setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
  if (ret < 0) return -1;

  /* Set up the server socket to accept connections from any address
   * at port 8080.
   */
  bzero( (void *)&servaddr, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
  servaddr.sin_port = htons( 8080 );

  /* Bind the socket with the prior servaddr structure */
  ret = bind( listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr) );
  if (ret < 0) return -1;

  listen(listenfd, 1);

  return 0;
}


/*
 *  checkHttpServer()
 *
 *  Check the server socket to see if any clients have connected.  This
 *  function uses the select call to timeout if no clients have connected.
 *  The function is re-called later to see if any connections have been
 *  made.
 *
 */

void checkHttpServer( void )
{
  fd_set rfds;
  struct timeval tv;
  int ret = -1;
  int connfd;
  socklen_t clilen;
  struct sockaddr_in cliaddr;

  /* We're listening only on our server fd (listenfd) */
  FD_ZERO( &rfds );
  FD_SET( listenfd, &rfds );

  /* Set up a one second timeout */
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  /* Call select.  This function will await until a read event is available
   * on the defined sockets (one in this case) or our timeout period has
   * expired.
   */
  ret = select( listenfd+1, &rfds, NULL, NULL, &tv );

  if (ret > 0) {

    /* Check to see if our read event was found.  Note that in this case
     * we're using the read event as an indication of a new socket on
     * our listening socket (no new data is available).
     */
    if (FD_ISSET(listenfd, &rfds)) {

      /* Accept the connection and respond to it with handleConnection. */
      clilen = sizeof(cliaddr);
      connfd = accept( listenfd, (struct sockaddr *)&cliaddr, &clilen );
      if (connfd > 0) {
        handleConnection( connfd );
        close( connfd );
      }

    } else {

      /* Some kind of error, reinitialize... */
      initHttpServer();

    }

  } else if (ret < 0) {

      /* Some kind of error, reinitialize... */
      initHttpServer();

  } else {

    // timeout -- no action.

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

