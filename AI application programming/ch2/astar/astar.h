/*
 *  Astar search types and symbolics.
 *
 *  mtj@cogitollc.com
 *
 */

struct node_s;

typedef struct node_s {
  struct node_s *parent;
  double g;
  double h;
  double f;
  int    x;
  int    y;
} node_t;

#define MAX_LIST	500

typedef struct {
  int    numElems;
  node_t *elem[MAX_LIST];
} list_t;

#define X_MAX	15
#define Y_MAX   15

#define MIN_COST	(double)1.0
#define ALPHA		(double)0.5

#define GET		0
#define REMOVE		1

extern node_t *start_node_p;

node_t *allocateNode( int, int );
node_t *findSuccessorNode( node_t *, int );
double calc_h( node_t * );
double calc_g( node_t * );
void showBestPath( node_t * );

void listInit( list_t * );
int  listEmpty( list_t * );
node_t *listFindBest( list_t * );
int  listPresent( list_t *, int x, int y );
void listAdd( list_t *, node_t * );
node_t *listGet( list_t *, int, int, int );
void cleanup( void );

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
