/*
 *  Astar search main program.
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "astar.h"

int grid[Y_MAX][X_MAX]= {
  { '+','-','-','-','-','-','-','-','-','-','-','-','-','-','+'},
  { '|',' ',' ',' ',' ','-','+','-',' ',' ',' ','-','+','-','|'},
  { '|',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ','|',' ','|'},
  { '|',' ','-','+','-',' ',' ',' ','-','+','-',' ',' ',' ','|'},
  { '|',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ','|',' ','|'},
  { '|',' ',' ',' ',' ','-','+','-',' ',' ',' ','-','+','-','|'},
  { '|',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ','|',' ','|'},
  { '|',' ','-','+','-',' ',' ',' ','-','+','-',' ',' ',' ','|'},
  { '|',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ','|',' ','|'},
  { '|',' ',' ',' ',' ','-','+','-',' ',' ',' ','-','+','-','|'},
  { '|',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ','|',' ','|'},
  { '|',' ','-','+','-',' ',' ',' ','-','+','-',' ',' ',' ','|'},
  { '|',' ',' ','|',' ',' ','|',' ',' ','|',' ',' ','|',' ','|'},
  { '|',' ',' ',' ',' ','-','+','-',' ',' ',' ','-','+','-','|'},
  { '+','-','-','-','-','-','-','-','-','-','-','-','-','-','+'}
};

const int goal_x = 13;
const int goal_y = 7;
const int start_x = 1;
const int start_y = 7;

#if 0
int grid[Y_MAX][X_MAX]= {
  { '+','-','-','-','-','-','-','-','-','-','-','-','-','-','+'},
  { '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|'},
  { '|',' ','+','-','+','-','-','-','+','-','-','-','+',' ','|'},
  { '|',' ','|',' ','|',' ',' ',' ','|',' ',' ',' ','|',' ','|'},
  { '|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|'},
  { '|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|'},
  { '|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|'},
  { '|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|'},
  { '|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|'},
  { '|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|'},
  { '|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|'},
  { '|',' ','|',' ',' ',' ','|',' ',' ',' ','|',' ',' ',' ','|'},
  { '|',' ','+','-','-','-','+','-','-','-','+','-','-',' ','|'},
  { '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|'},
  { '+','-','-','-','-','-','-','-','-','-','-','-','-','-','+'}
};

const int goal_x = 3;
const int goal_y = 3;
const int start_x = 1;
const int start_y = 1;

#endif

node_t *start_node_p;

list_t openList_p;
list_t closedList_p;

int main()
{
  node_t *curNode_p;

  listInit( &openList_p );
  listInit( &closedList_p );

  start_node_p = allocateNode(start_x, start_y);

  /* Begin with our start node as the initial node */
  listAdd( &openList_p, start_node_p );

  while ( !listEmpty(&openList_p) ) {

    curNode_p = listFindBest( &openList_p );
    (void)listGet( &openList_p, curNode_p->x, curNode_p->y, REMOVE );
    listAdd( &closedList_p, curNode_p );

    if ((curNode_p->x == goal_x) && (curNode_p->y == goal_y)) {

      showBestPath( curNode_p );
      cleanup();

      return 0;

    } else {

      int i;
      node_t *successor_p;
      node_t *temp;

      for (i = 0 ; i < 4 ; i++) {

        successor_p = findSuccessorNode( curNode_p, i );

        if (successor_p != (node_t *)NULL) {

          successor_p->h = calc_h( successor_p );
          successor_p->g = curNode_p->g + calc_g( curNode_p );
          successor_p->f = successor_p->g + successor_p->h;

          if (listPresent(&openList_p, successor_p->x, successor_p->y)) {
          
            temp = listGet( &openList_p, successor_p->x, successor_p->y, GET );

            if (temp->f < successor_p->f) {

              free( successor_p );
              continue;

            }

          }

          if (listPresent(&closedList_p, successor_p->x, successor_p->y)) {
          
            temp = listGet( &closedList_p, successor_p->x, successor_p->y, GET );

            if (temp->f < successor_p->f) {

              free( successor_p );
              continue;

            }

          }

          temp = listGet( &openList_p, 
                           successor_p->x, successor_p->y, REMOVE );
          if (temp) free(temp);

          temp = listGet( &closedList_p, 
                           successor_p->x, successor_p->y, REMOVE );
          if (temp) free(temp);

          successor_p->parent = (node_t *)curNode_p;
          listAdd( &openList_p, successor_p );

        }

      }

    }

  }
  
  /* Solution not found */
  printf("Solution not found.\n");

  cleanup();

  return 0;
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
