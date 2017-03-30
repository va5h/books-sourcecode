/*
 *  Configuration File Parser Types and Symbolics
 *
 *  ./software/ch11/prsconf.h
 *
 *  mtj@cogitollc.com
 *
 */

#ifndef __PRSCONF_H
#define __PRSCONF_H

#define MAX_URL_SIZE            80
#define MAX_SEARCH_ITEM_SIZE    40
#define MAX_SEARCH_STRINGS      10

#define MAX_GROUPS		20

#define MAX_MONITORS    20

#define MAX_LINE        300

#define MAX_NEWS_MSG	(64*1024)

#define MONITOR_PARSE   0
#define FEEDS_PARSE     1
#define GROUPS_PARSE    2

typedef struct {
  int  active;
  char groupName[MAX_URL_SIZE+1];
  int  lastMessageRead;
  char searchString[MAX_SEARCH_STRINGS][MAX_SEARCH_ITEM_SIZE+1];
  int  numSearchStrings;
} groupEntryType;

typedef struct {
  char           url[MAX_URL_SIZE];
  groupEntryType groups[MAX_GROUPS];
} feedEntryType;

typedef struct {
  int  active;
  char url[MAX_URL_SIZE];
  char urlName[MAX_SEARCH_ITEM_SIZE+1];
  int  length;
  int  changed;
  int  shown;
} monitorEntryType;

#endif /* __PRSCONF_H */


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

