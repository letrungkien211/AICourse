/*
 *  Astar search list functions.
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <assert.h>
#include "astar.h"

void listInit( list_t *list_p )
{
  int i;

  list_p->numElems = 0;

  for (i = 0 ; i < MAX_LIST ; i++) {
    list_p->elem[i] = (node_t *)NULL;
  }

  return;
}


void listPrint( list_t *list_p )
{
  int i;

  for (i = 0 ; i < MAX_LIST ; i++) {
    if (list_p->elem[i] != 0) {
      printf("(%d,%d) ", list_p->elem[i]->x, list_p->elem[i]->y);
    }
  }
  printf("\n");

}


int listEmpty( list_t *list_p )
{
  if (list_p->numElems == 0) return 1;
  else return 0;
}


node_t *listFindBest( list_t *list_p )
{
  int i;
  int best = -1;

  for (i = 0 ; i < MAX_LIST ; i++) {

    if (list_p->elem[i] != 0) {
      best = i++;
      break;
    }

  }

  for ( ; i < MAX_LIST ; i++) {

    if (list_p->elem[i] != 0) {
      if (list_p->elem[i]->f < list_p->elem[best]->f) {
        best = i;
      }
    }

  }

  return list_p->elem[best];
}


int listPresent( list_t *list_p, int x, int y )
{
  int i;

  for (i = 0 ; i < MAX_LIST ; i++) {
    if (list_p->elem[i]) {
      if ((list_p->elem[i]->x == x) && (list_p->elem[i]->y == y)) return 1;
    }
  }

  return 0;
}


void listAdd( list_t *list_p, node_t *elem_p )
{
  int i;

  for (i = 0 ; i < MAX_LIST ; i++) {

    if (list_p->elem[i] == (node_t *)NULL) {
      list_p->elem[i] = elem_p;
      list_p->numElems++;
      break;
    }

  }

  return;
}


node_t *listGet( list_t *list_p, int x, int y, int remove )
{
  int i;
  node_t *node = (node_t *)NULL;

  for (i = 0 ; i < MAX_LIST ; i++) {

    if (list_p->elem[i]) {

      if ((list_p->elem[i]->x == x) && (list_p->elem[i]->y == y)) {

        node = list_p->elem[i];
        if (remove) {
          list_p->elem[i] = (node_t *)NULL;
          list_p->numElems--;
        }
        break; 

      }

    } 

  }

  return node;
}


/*
 * Copyright (c) 2003-2005 Charles River Media.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, is hereby granted without fee provided
 * that the following conditions are met:
 *
 *   1.  Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the
 *       following disclaimer.
 *   2.  Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the
 *       following disclaimer in the documentation and/or other
 *       materials provided with the distribution.
 *   3.  Neither the name of Charles River Media nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior
 *       written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CHARLES RIVER MEDIA AND CONTRIBUTORS
 * 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CHARLES
 * RIVER MEDIA OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARAY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
