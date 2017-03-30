/*
 *  WebAgent Main Function
 *
 *  ./software/ch11/main.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "protos.h"
#include "prsconf.h"

/*
 *  main()
 *
 *  Main program for WebAgent.
 *
 */

int main()
{
  int timer=0, ret, i;

  extern monitorEntryType monitors[];

  /* Parse the configuration file */
  ret = parseConfigFile( "config" );

  if (ret != 0) {
    printf("Error reading configuration file\n");
    exit(0);
  }

  /* Start the HTTP server */
  initHttpServer();

  while (1) {

    /* Check the news and monitor sites every 10 minutes */
    if ((timer % 600) == 0) {

      printf("Checking News...\n");

      /* Check news from the defined net news server */
      checkNewsSources();

      printf("Monitoring...\n");

      /* Check to see if any defined web-sites have been updated */
      for (i = 0 ; i < MAX_MONITORS ; i++) {
        if (monitors[i].active) monitorSite( i );
      }

      printf("Finished Checking...\n");

    }

    /* Check to see if a client has made a request */
    checkHttpServer();

    timer++;

  }

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

