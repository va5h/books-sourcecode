/*
 *  HTML Generation Files
 *
 *  ./software/ch11/newsgen.c
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "protos.h"
#include "nqueue.h"
#include "prsconf.h"

const char *prologue={
  "<HTML><HEAD><TITLE>WebAgent</TITLE></HEAD>"
  "<BODY TEXT=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000EE\""
  "vlink=\"#551A8B\" alink=\"#FF0000\">"
  "<BR><font face=\"Bauhaus Md BT\"><font color=\"#000000\">"
};


const char *epilogue={
  "</BODY></HTML>\n"
};


/*
 *  emitNews()
 *
 *  Emit the HTML content that displays the available news that can be
 *  viewed.
 *
 */

void emitNews( int connfd )
{
  int  i;
  char line[MAX_LINE+1];
  elementType *walker;

  extern monitorEntryType monitors[];
  extern feedEntryType feed;
  extern elementType head;

  /* Emit the standard response header */
  emitHTTPResponseHeader( connfd );

  /* Next, emit the standard prologue and content headers */
  write( connfd, prologue, strlen(prologue));

  strcpy(line, 
          "<H1>Web Agent Results</H1></font></font><BR><BR>");
  write( connfd, line, strlen(line));

  /* First -- emit the web sites that have changed */

  /* Create an HTML table */
  strcpy(line, "<center><table BORDER=3 WIDTH=100% NOSAVE><tr>\n");
  write( connfd, line, strlen(line));

  /* Walk through each of the active monitors that have changed and emit
   * a row with the relevent data.
   */
  for (i = 0 ; i < MAX_MONITORS ; i++) {

    if ((monitors[i].active) && (monitors[i].changed)) {

      sprintf(line, "<tr><td><font size=+1>%s</font></td>\n"
                    "<td><font size=+1><a href=\"%s\">%s</a>"
                    "</font></td></tr>\n",
               monitors[i].urlName, monitors[i].url, monitors[i].url);

      write( connfd, line, strlen(line));

      monitors[i].shown = 1;

    }

  }

  /* Close out the table */
  strcpy(line, "</tr></table></center><BR><BR>\n");
  write( connfd, line, strlen(line));

  /* Second -- emit the collected news items */

  /* Initialize a temporary pointer with the head of the news list */
  walker = head.next;

  /* If this news walker is not null, continue to process each item */
  if (walker) {

    /* Create a new table for the news elements */
    strcpy(line, "<center><table BORDER=3 WIDTH=100% NOSAVE><tr>\n");
    write( connfd, line, strlen(line));

    /* While more news items are available, continue through the list */
    while (walker) {

      /* Construct and send a string containing the table row for the news 
       * item.
       */
      sprintf(line, "<tr><td><font size=+1>%s</font></td>\n"
                    "<td><font size=+1><a href=\"%s\">%s</a></font></td>"
                    "<td><font size=+1>%s</font></td></tr>",
                    feed.groups[walker->group].groupName,
                    walker->link,
                    walker->subject, 
                    walker->msgDate );
      write( connfd, line, strlen(line));

      /* Identify that this particular news item has been shown to the user */
      walker->shown = 1;

      /* Walk to the next element in the news list */
      walker = walker->next;

    }

    /* Emit the end of the table tags */
    strcpy(line, "</tr></table></center>\n");
    write( connfd, line, strlen(line));

  }

  /* Generate a button for clearing the news.  The file associated with the
   * button is '/clear'.
   */
  strcpy(line, "<FORM METHOD=\"POST\" ACTION=/clear\">");
  write( connfd, line, strlen(line));

  strcpy(line, "<BR><BR><INPUT TYPE=\"submit\" VALUE=\"Mark Read\"><BR>\n");
  write( connfd, line, strlen(line));

  /* Emit the epilogue tags */
  write( connfd, epilogue, strlen(epilogue));

  return;
}


/*
 *  emitConfig()
 *
 *  Generate the HTML content that displays the WebAgent configuration.
 *
 */

void emitConfig( int connfd )
{
  char line[MAX_LINE+1];
  int i, j;

  extern monitorEntryType monitors[];
  extern feedEntryType feed;

  /* Emit the standard HTTP response message header */
  emitHTTPResponseHeader( connfd );

  write( connfd, prologue, strlen(prologue));

  /* First -- emit the web site monitoring configuration */

  strcpy(line, "<H1>Configuration</H1></font></font><BR><BR>");
  write( connfd, line, strlen(line));

  strcpy(line, "<font size=+2>Sites to Monitor</font><BR><BR>");
  write( connfd, line, strlen(line));

  /* Emit HTML tags to construct a table for the monitored web-sites */
  strcpy(line, "<center><table BORDER=3 WIDTH=100% NOSAVE><tr>\n");
  write( connfd, line, strlen(line));

  /* For each active web-site monitor, emit a row with the relevent data */
  for (i = 0 ; i < MAX_MONITORS ; i++) {

    if (monitors[i].active) {

      sprintf(line, "<tr><td><font size=+1>%s</font></td><td>"
                    "<font size=+1>%s<font></td></tr>\n",
               monitors[i].urlName, monitors[i].url);
      write( connfd, line, strlen(line));

    }

  }

  /* Emit the tags to close out the table */
  strcpy(line, "</tr></table></center><BR><BR>\n");
  write( connfd, line, strlen(line));

  /* Second -- Emit the news gathering configuration */

  sprintf(line, 
    "<H2>Feed %s</H2><BR><BR>\n", feed.url);
  write( connfd, line, strlen(line));

  /* Emit the tags to construct a table */
  strcpy(line, "<center><table BORDER=3 WIDTH=100% NOSAVE><tr>\n");
  write( connfd, line, strlen(line));

  /* Walk through the configured news groups */
  for (i = 0 ; i < MAX_GROUPS ; i++) {

    /* Only check active groups */
    if (feed.groups[i].active) {

      /* Emit a row for the group including the group name and the
       * search strings associated with the group.
       */
      sprintf(line, "<tr><td><font size=+1>Group %s</font></td>\n", 
               feed.groups[i].groupName);
      write( connfd, line, strlen(line));

      strcpy(line, "\n<td><font size=+1>");
      if (feed.groups[i].numSearchStrings > 0) {
        for (j = 0 ; j < feed.groups[i].numSearchStrings ; j++) {
          if (j > 0) strcat(line, ", ");
          strcat(line, feed.groups[i].searchString[j]);
        }
      } else {

        /* If no search strings are present, emit '[*]' */
        strcat(line, "[*]");
      }

      strcat(line, "</font></td></tr>\n");
      write( connfd, line, strlen(line) );

    }

  }

  /* Close out the HTML table */
  strcpy(line, "</tr></table></center><BR><BR>\n");
  write( connfd, line, strlen(line));

  write( connfd, epilogue, strlen(epilogue));

  return;
}


/*
 *  emitArticle()
 *
 *  Generate the HTML that displays the news item selected by the user.
 *
 */

void emitArticle( int connfd, char *filename )
{
  int group, article, count, result;
  news_t news;
  char line[MAX_LINE+1];

  extern feedEntryType feed;

  /* Emit the standard HTTP response header */
  emitHTTPResponseHeader( connfd );

  write( connfd, prologue, strlen(prologue));

  /* Construct a filename based upon the group and article numbers */
  sscanf(filename, "/art%d_%d", &group, &article);

  /* Build a news structure in preparation for using the NNTP API. */
  news.msg = (char *)malloc(MAX_NEWS_MSG+1);
  bzero( news.msg, MAX_NEWS_MSG+1 );
  news.msgLen = MAX_NEWS_MSG;

  /* Prune the URL to a simple FQDN */
  prune( feed.url, line );

  /* Connect to the defined NNTP server */
  count = nntpConnect( line );

  if (count == 0) {

    /* Set the group and last message read per the group structure */
    count = nntpSetGroup( feed.groups[group].groupName, article-1 );

    if (count > 0) {

      /* Retrieve a message -- will be the last message read as initialized
       * in nntpSetGroup.
       */
      result = nntpRetrieve( &news, MAX_NEWS_MSG );

      if (result > 0) {

        /* Parse the news item retrieved */
        result = nntpParse( &news, FULL_PARSE );

        if (result == 0) {

          /* Construct the tags to emit the article information to the client.
           */
          sprintf( line, "<font size=+1>Subject  : %s\n</font><BR><BR>", 
                    news.subject );
          write( connfd, line, strlen(line) );

          sprintf( line, "<font size=+1>Sender   : %s\n</font><BR><BR>", 
                    news.sender );
          write( connfd, line, strlen(line) );

          sprintf( line, "<font size=+1>Group    : %s\n</font><BR><BR>", 
                    feed.groups[group].groupName );
          write( connfd, line, strlen(line) );

          sprintf( line, "<font size=+1>Msg Date : %s\n</font>"
                         "<BR><BR><hr><PRE>", 
                    news.msgDate );
          write( connfd, line, strlen(line) );

          write( connfd, news.bodyStart, strlen(news.bodyStart) );

          sprintf(line, "</PRE><BR><BR>End of Message\n<BR><BR>");
          write( connfd, line, strlen(line) );

        } else {

          /* Write error */
          printf("Parse error\n");

        }

      }

    }

  }

  write( connfd, epilogue, strlen(epilogue));

  /* Free our news structure */
  free( news.msg );

  /* Disconnect from the NNTP server */
  nntpDisconnect();

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

