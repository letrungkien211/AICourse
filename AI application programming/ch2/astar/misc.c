/*
 *  Astar search miscellaneous functions.
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "astar.h"

#define abs(x)	(((x) >= 0.0) ? (x) : -(x))

node_t *allocateNode( int x, int y )
{
  node_t *node_p;

  node_p = malloc( sizeof(node_t) );
  
  node_p->g = node_p->h = node_p->f = (double)0.0;
  node_p->parent = (node_t *)NULL;

  node_p->x = x;  node_p->y = y;

  return node_p;
}


const struct {
  int x;
  int y;
} succ[4]={{0, -1}, {0, 1}, {1, 0}, {-1, 0}};

node_t *findSuccessorNode( node_t *curNode_p, int i )
{
  node_t *successor_p = (node_t *)NULL;
  int x, y;

  extern int grid[Y_MAX][X_MAX];

  x = curNode_p->x + succ[i].x;
  y = curNode_p->y + succ[i].y;

  assert(i<4);

  if ( grid[y][x] == ' ' ) successor_p = allocateNode( x, y );

  return successor_p;
}


double calc_h( node_t *node_p )
{
  double h;

  extern const int goal_x, goal_y;

  assert(node_p);

  h = (double)( MIN_COST * 
                (abs((double)node_p->x-(double)goal_x) + 
                 abs((double)node_p->y-(double)goal_y)) );

  return h;
}


double calc_g( node_t *node_p )
{
  double g;

  assert(node_p);

  g = 1.0 + ALPHA * (node_p->g - 1.0);

  return g;
}


void showBestPath( node_t *walker )
{
  int x, y;

  extern int grid[Y_MAX][X_MAX];
  extern const int goal_x, goal_y;

  grid[goal_y][goal_x] = 'G';

  walker = (node_t *)walker->parent;

  while (walker->parent != (node_t *)NULL) {

    grid[walker->y][walker->x] = '.';
    walker = (node_t *)walker->parent;

  }

  grid[start_node_p->y][start_node_p->x] = 'S';

  printf("\n");
  for (y = 0 ; y < Y_MAX ; y++) {
    for (x = 0 ; x < X_MAX ; x++) {
      printf("%c ", grid[y][x]);
    }
    printf("\n");
  }
  printf("\n");

  return;
}


void cleanup( void )
{
  int i;

  extern list_t openList_p;
  extern list_t closedList_p;

  for (i = 0 ; i < MAX_LIST ; i++) {
    if (openList_p.elem[i]) free(openList_p.elem[i]);
    if (closedList_p.elem[i]) free(closedList_p.elem[i]);
  }

  return;
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
