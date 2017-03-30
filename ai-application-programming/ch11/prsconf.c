/*
 *  Configuration File Processing Functions
 *
 *  ./software/ch11/prsconf.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prsconf.h"
#include "protos.h"

feedEntryType feed;
int curGroup = 0;

monitorEntryType monitors[MAX_MONITORS];
int curMonitor = 0;


/*
 *  parseString()
 *
 *  This function parses arbitrary strings from the file.
 *
 */

char *parseString( char *line, char *string )
{
  int j=0;

  /* We should be sitting on a separator, if not, something has gone awry
   * and we exit.
   */
  if (*line != ';') {
    *line = 0;
    return line;
  }
  line++;

  /* Skip any spaces present in the input string */
  while (*line == ' ') line++;

  /* Copy the string, converting to lowercase */
  while ((*line != ';') && (*line != 0x0a)) {
    string[j++] = tolower(*line++);
    if (j == MAX_SEARCH_ITEM_SIZE-1) break;
  }
  string[j] = 0;

  /* Skip any additional whitespace in the source string */
  while ((*line != ';') && (*line != 0)) line++;

  return( line );
}


/*
 *  parseURLorGroup
 *
 *  This function parses a sequence of characters (commonly representing a
 *  URL or group) until a separator symbol is found.  The characters are
 *  copied from line to url.
 */

char *parseURLorGroup( char *line, char *url )
{
  int i = 0;

  /* Search for the ';' or ' ' seperator */
  while ((*line != ' ') && (*line != ';') && (*line != 0x0a)) {
    url[i++] = *line++;
    if (i == MAX_URL_SIZE-1) break;
  }
  url[i] = 0;

  /* Skip any white-space in the original string. */
  while ((*line != ';') && (*line != 0) && (*line != 0x0a)) i++;

  return( line );
}


/*
 *  parseConfigFile()
 *
 *  High level configuration file parser.  This simple state-machine driven
 *  design parses the three distinct file sections.
 *
 */

int parseConfigFile( char *filename )
{
  FILE *fp;
  char line[MAX_LINE+1], *cur;
  int parse, i;

  /* Initialize the operating configuration structures */
  bzero( &feed, sizeof(feed) );
  bzero( monitors, sizeof(monitors) );

  /* Open the filename defined by the caller */
  fp = fopen(filename, "r");

  if (fp == NULL) return -1;

  /* Walk through each line of the file until the end */
  while( !feof(fp) ) {

    /* Get a line from the file */
    fgets( line, MAX_LINE, fp );

    /* If we've reached the end, break */
    if (feof(fp)) break;

    /* If the line is a comment or blank, continue to get the next line */
    if      (line[0] == '#') continue;
    else if (line[0] == 0x0a) continue;

    /* Check the configuration file markers, when one is found, change the
     * parsing state accordingly.
     */
    if        (!strncmp(line, "[monitor]", 9)) {
      parse = MONITOR_PARSE;
    } else if (!strncmp(line, "[feeds]", 7)) {
      parse = FEEDS_PARSE;
    } else if (!strncmp(line, "[groups]", 8)) {
      parse = GROUPS_PARSE;
    } else {

      if (parse == MONITOR_PARSE) {

        /* For monitor parsing, we're looking or URLS.  These must be
         * preceded by the 'http://' protocol specification.  We parse
         * the URL and then the subsequent strings separately.
         */

        if (!strncmp(line, "http://", 7)) {
          cur = parseURLorGroup( line, monitors[curMonitor].url );
          parseString( cur, monitors[curMonitor].urlName );
          monitors[curMonitor].active = 1;
          curMonitor++;
        } else return -1;

      } else if (parse == FEEDS_PARSE) {

        /* For feeds parsing, we're looking for an NNTP server URL */

        if (!strncmp(line, "nntp://", 7)) {
          cur = parseURLorGroup( line, feed.url );
        } else return -1;

      } else if (parse == GROUPS_PARSE) {

        /* For groups parsing, we're looking for a URL and then a set of
         * search strings.
         */

        cur = parseURLorGroup( line, feed.groups[curGroup].groupName );

        i = 0;
        while (*cur) {
          cur = parseString( cur, feed.groups[curGroup].searchString[i] );
          if (strlen(feed.groups[curGroup].searchString[i])) i++;
          if (i == MAX_SEARCH_STRINGS) break;
        }
        feed.groups[curGroup].numSearchStrings = i;
        feed.groups[curGroup].active = 1;

        curGroup++;

      }

    }

  }

  /* Now that we've read the configuration file, read the group status file
   * to preload the last message read (for any groups that are in both the
   * configuration file and group status file).
   */
  readGroupStatus();

#if 1
  { int i, j;

  for (i = 0 ; i < curMonitor ; i++) {
    printf("Monitor %s [%s]\n", monitors[i].url, monitors[i].urlName);
  }

  printf("feed %s\n", feed.url);

  for (i = 0 ; i < curGroup ; i++) {

    if (feed.groups[i].active) {

      printf("group %s", feed.groups[i].groupName);

      printf(" -- search strings : ");

      for (j = 0 ; j < MAX_SEARCH_STRINGS ; j++) {

        if (strlen(feed.groups[i].searchString[j])) 
          printf("%s ", feed.groups[i].searchString[j]);

      }

      printf("\n");

    }

  }

  }
#endif

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

