/*
 *  Reinforcement Learning Application
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include "grid.h"

/*
 * Given an action, return the y,x deltas for movement.
 */
const loc_t dir[MAX_ACTIONS]={
  { -1,  0},	/* N  */
  { -1,  1},    /* NE */
  {  0,  1},    /* E  */
  {  1,  1},    /* SE */
  {  1,  0},    /* S  */
  {  1, -1},    /* SW */
  {  0, -1},    /* W  */
  { -1, -1}     /* NW */
};

/*
 * Test grid.
 */
int grid[Y_MAX][X_MAX]={
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { -1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
  { -1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
  { -1,  1,  1,  1,  1,  1,  1, -1, 99, -1 },
  { -1,  1,  1,  1,  1,  1,  1, -1,  9, -1 },
  { -1,  1,  1,  1,  1,  9,  9, -1,  9, -1 },
  { -1,  1,  1,  1,  1,  9,  9, -1,  9, -1 },
  { -1,  1,  1,  1,  1,  9,  9, -1,  9, -1 },
  { -1, -1,  1,  1,  1,  1,  9,  9,  9, -1 },
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
};

/*
 *  Start and End locations.
 */
loc_t start={1, 1};
loc_t end={3, 8};



/*
 * State space (Q values) for each location in the grid.
 */
stateAction_t stateSpace[Y_MAX][X_MAX];

/*
 *  explore defines rate of exploration
 *    (where 1-explore is exploitation rate).
 *
 *  actionSelection is
 *    GREEDY   -- Follow max Q values.
 *    P_GREEDY -- Probabilistically follow Q values.
 *
 */
double explore = EXPLORE_RATE;
int actionSelection = P_GREEDY;


/*
 *  initStateSpace()
 *
 *  Initialize the Grid World.
 *
 */

void initStateSpace( void )
{
  int x_state, y_state, action;

  for ( y_state = 0 ; y_state < Y_MAX ; y_state++ ) {

    for ( x_state = 0 ; x_state < X_MAX ; x_state++ ) {

      for (action = 0 ; action < MAX_ACTIONS ; action++) {

        stateSpace[y_state][x_state].Q[action] = 0.0;

      }

      stateSpace[y_state][x_state].sumQ = 0.0;
      stateSpace[y_state][x_state].maxQ = 0.0;
    
    }

  }

  return;
}


/*
 *  findMaxQ()
 *
 *  Find and store the maximum Q value for a given location.
 *
 */

void findMaxQ( int y, int x )
{
  int i;

  stateSpace[y][x].maxQ = 0.0;

  for (i = 0 ; i < MAX_ACTIONS ; i++) {

    if (stateSpace[y][x].Q[i] > stateSpace[y][x].maxQ) {

      stateSpace[y][x].maxQ = stateSpace[y][x].Q[i];

    }

  }

  return;
}


/*
 *  updateSum()
 *
 *  Update the sum of Q variables for the given grid location.
 *
 */

void updateSum( int y, int x )
{
  int i;

  stateSpace[y][x].sumQ = 0.0;

  for (i = 0 ; i < MAX_ACTIONS ; i++) {

    stateSpace[y][x].sumQ += stateSpace[y][x].Q[i];

  }

  return;
}


/*
 *  legalMove()
 *
 *  Identify if the current action is legal for the given location.
 *
 */

int legalMove( int y_state, int x_state, int action )
{
  int y = y_state + dir[action].y;
  int x = x_state + dir[action].x;

  if (grid[y][x] < 0) return 0;
  else return 1;
}


/*
 *  getRandomAction()
 * 
 *  For the given location, return a random action that will result in
 *  a legal move.
 *
 */

int getRandomAction( int y, int x )
{
  int action;

  do {
    action = getRand( MAX_ACTIONS );
  } while (!legalMove(y, x, action));

  return action;
}


/*
 *  chooseAction()
 *
 *  For the given location, return a legal action given either the
 *  GREEDY or P_GREEDY action selection policy.
 *
 */

int chooseAction( int y, int x )
{
  int action;
  int count;

  if (actionSelection == GREEDY) {

    findMaxQ(y, x);

    for (action = 0 ; action < MAX_ACTIONS ; action++) {

      if (stateSpace[y][x].maxQ == stateSpace[y][x].Q[action]) {
        if (legalMove(y, x, action)) return(action);
      }

    }

    assert(0);

  } else if (actionSelection == P_GREEDY) {

    if ((EXPLORE_RATE > getSRand()) ||
        (stateSpace[y][x].sumQ == 0.0)) {

      /* Explore! */
      action = getRandomAction(y, x);

    } else {

      double prob;

      /* Exploit! */
      action = getRandomAction(y, x);

      for (count = 0 ; count < MAX_ACTIONS ; count++) {

        prob = (stateSpace[y][x].Q[action] / stateSpace[y][x].sumQ);

        if (legalMove(y, x, action)) {
          if (prob > getSRand()) {
            return(action);
          }
        }

        if (++action == MAX_ACTIONS) action = 0;

      }

      /* Went through the possible actions, but did not pick one.  At
       * this point, we'll just pick one randomly.
       */
      if (count == MAX_ACTIONS) {
        action = getRandomAction(y, x);
      }

    }

  } else {
    assert(0);
  }

  return action;
}


/*
 *  updateFunction()
 *
 *  For the given location and action, perform the Q-Learning update
 *  function (adjusting the state/action Q value).
 *
 */

void updateFunction( int y, int x, int action )
{
  int newy = y + dir[action].y;
  int newx = x + dir[action].x;
  double reward;

  reward = (double)grid[newy][newx] + 0.1;

  /* Update the maxQ value for the state */
  findMaxQ( newy, newx );

  /* Q_Learning */
  stateSpace[y][x].Q[action] += 
    LEARNING_RATE * (reward + (DISCOUNT_RATE * stateSpace[newy][newx].maxQ) -
                      stateSpace[y][x].Q[action]);

  /* Update the payoff sum */
  updateSum( y, x );

  return;
}


/*
 *  showPath()
 *
 *  Show the agent's path through the Grid world using the GREEDY
 *  action selection policy.
 *
 */

void showPath( void )
{
  int x, y, action;

  actionSelection = GREEDY;
  
  x = start.x; y = start.y;

  printf("\n");

  for ( y = 0 ; y < Y_MAX ; y++ ) {
    for ( x = 0 ; x < X_MAX ; x++ ) {
      printf("%2d ", grid[y][x]);
    }
    printf("\n");
  }
  printf("\n");

  x = start.x; y = start.y;

  while (1) {

    grid[y][x] = 0;

    if ((x == end.x) && (y == end.y)) break;

    action = chooseAction( y, x );

    x += dir[action].x;
    y += dir[action].y;

  }

  printf("\n");

  for ( y = 0 ; y < Y_MAX ; y++ ) {
    for ( x = 0 ; x < X_MAX ; x++ ) {
      printf("%2d ", grid[y][x]);
    }
    printf("\n");
  }
  printf("\n");

  return;
}


/*
 *  main()
 *
 *  Main loop for the Q-Learning grid world example.  Work through the
 *  grid for MAX_EPOCHS.
 *
 */

int main()
{
  long epochs;
  int x, y, action;

  x = start.x; y = start.y;

  srand(time(NULL));

  initStateSpace();

  for ( epochs = 0 ; epochs < MAX_EPOCHS ; epochs++ ) {

    action = chooseAction( y, x );

    updateFunction( y, x, action );

    x += dir[action].x;
    y += dir[action].y;

    if ((x == end.x) && (y == end.y)) {
      x = start.x; y = start.y;
    }

  }

  showPath();

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
