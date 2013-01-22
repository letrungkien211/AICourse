/*
 *  Breadth-First-Search Implementation.
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define NUM_NODES	10
#define MAX_QUEUE	10

/*            FROM       TO */
int adjMatrix[NUM_NODES][NUM_NODES];
int visited[NUM_NODES];

int queue[MAX_QUEUE];
int write,read;

#define WRAP(x)		((x == (MAX_QUEUE)-1) ? 0 : x+1)


void initAdjMatrix( void )
{
  memset ( (void *)adjMatrix, 0, sizeof(adjMatrix) );
  memset ( (void *)visited, 0, sizeof(visited) );

  return;
}


void makeEdge( int from, int to )
{
  assert(from < NUM_NODES );
  assert(to < NUM_NODES );

  adjMatrix[from][to] = 1;

}


void initQueue( void )
{
  write = read = 0;

  return;
}


int empty( void )
{
  if (write == read) return 1;
  else return 0;
}


void enqueue( int elem )
{
  queue[write] = elem;
  write = WRAP(write);

  return;
}


int dequeue( void )
{
  int elem;

  assert( !empty() );
  elem = queue[read];
  read = WRAP(read);

  return elem;
}


void dfs( int start, int goal )
{
  int node, i;

  enqueue( start );

  while (!empty()) {

    /* Open a new node */
    node = dequeue();

    if (node == goal) {
      printf("%d Goal\n", node);
      return;
    }

    if (visited[node]) continue;
    else visited[node] = 1;

    printf("%d ", node);

    /* Enqueue each of the children of the current node
     * onto the queue.
     */
    for (i = 0 ; i < NUM_NODES ; i++) {

      if ( adjMatrix[node][i] == 1 ) enqueue( i );

    }

  }

  printf("Goal not found.\n");

  return;
}


int main()
{
  initAdjMatrix();
  initQueue();

  /* Build our graph in the adj matrix */
  makeEdge( 1,2 ); 
  makeEdge( 1,3 ); 
  makeEdge( 1,4 ); 
  makeEdge( 2,5 ); 
  makeEdge( 3,5 ); 
  makeEdge( 3,6 ); 
  makeEdge( 4,7 ); 
  makeEdge( 5,8 ); 
  makeEdge( 5,9 ); 

  dfs( 1, 9 );

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
