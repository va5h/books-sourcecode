/*
 *  File I/O routines (for the news archive).
 *
 *  ./software/ch11/fileio.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "prsconf.h"

extern feedEntryType feed;

#define GRPSTS_FILE	"group.sts"

/*
 *  emitGroupStatus()
 *
 *  Emit the current news counts for each of the active groups.  This
 *  function is called from the HTTP server when the "Mark Read" button
 *  is pushed.
 *
 */

void emitGroupStatus( void )
{
  FILE *fp;
  int  i;

  /* Open the group status file */
  fp = fopen(GRPSTS_FILE, "w");

  /* For each active group within the feed, emit the groups and the last
   * message read for each.
   */
  for (i = 0 ; i < MAX_MONITORS ; i++) {

    if (feed.groups[i].active) {

      fprintf( fp, "%s : %d\n", 
                feed.groups[i].groupName, feed.groups[i].lastMessageRead );

    }

  }

  fclose(fp);

  return;
}


/*
 *  readGroupStatus()
 *
 *  Read the groups file in and load the news message id (last message
 *  read) and store it into the feed.groups array.  This forces the
 *  news monitor to ignore messages that have already been seen.
 *
 */

void readGroupStatus( void )
{
  FILE *fp;
  int  i, curMsg;
  char line[80];

  /* Initialize each of the groups as unread */
  for (i = 0 ; i < MAX_MONITORS ; i++) feed.groups[i].lastMessageRead = -1;

  /* Open the group file for read */
  fp = fopen(GRPSTS_FILE, "r");

  if (fp == NULL) return;

  /* While we've not reached the end of the file, read and parse each line */
  while (!feof(fp)) {

    /* Parse the line to group name and last message read. */
    fscanf( fp, "%s : %d\n", line, &curMsg );

    /* Walk through the active groups list looking for the group so that the
     * last message id can be applied to it.
     */
    for (i = 0 ; i < MAX_MONITORS ; i++) {

      if (feed.groups[i].active) {

        if (!strcmp(feed.groups[i].groupName, line)) {

          feed.groups[i].lastMessageRead = curMsg;
          break;

        }

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

