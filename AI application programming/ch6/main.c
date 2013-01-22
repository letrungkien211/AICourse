/*
 *  Classifer System implementation
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "clstypes.h"

classifier_t list[MAX_CLASSIFIERS];

unsigned char world[MAX_WORLD_Y][MAX_WORLD_X];

int agent_y, agent_x;

const dir_t dir[MAX_DIR]= { {-1, 0},    /* North */
                            { 0, 1},    /* East  */
                            { 1, 0},    /* South */
                            { 0,-1} };  /* West  */


unsigned char getRandomCharacter( void )
{
  const unsigned char alphabet[3] = {'0', '1', '#'};
  return( alphabet[getRand(3)] );
}




void readDetectors( unsigned char *condition )
{
  int i;

  for (i = 0 ; i < MAX_DIR ; i++) {
    int y = agent_y + dir[i].loc_y;
    int x = agent_x + dir[i].loc_x;
    condition[i] = (world[y][x] == '*') ? '1' : '0';
  }

  return;
}


int match( unsigned char cl, unsigned char cn )
{
  if      (cl == '#') return 1;
  else if (cl == cn) return 1;

  return 0;
}


void calculateSpecificity( int classifier )
{
  int i;

  list[classifier].specificity = 0.0;

  for (i = 0 ; i < MAX_COND_WIDTH ; i++) {
    if (list[classifier].condition[i] != '#') {
      list[classifier].specificity++;
    }
  }

  list[classifier].specificity /= (double)MAX_COND_WIDTH;

  return;
}


int createCoveringClassifier( int classifier, unsigned char *condition )
{
  int c, act;
  double minStrength = 99.0;

  if (classifier == -1) {

    /* Find the lowest strength classifier */
    for (c = 0 ; c < MAX_CLASSIFIERS ; c++) {
      if (list[c].strength < minStrength) {
        classifier = c;
        minStrength = list[c].strength;
      }
    }

  }

  /* Replace this classifier with a covering classifier */
  for (c = 0 ; c < MAX_COND_WIDTH ; c++) {
    if (getSRand() > 0.75) {
      list[classifier].condition[c] = condition[c];
    } else {
      list[classifier].condition[c] = '#';
    }
  }

  for (act = 0 ; act < MAX_ACTION_WIDTH ; act++) {
    list[classifier].action[act] = 0x30 + getRand(2);
  }

  list[classifier].strength = 1.0;
  list[classifier].episode = 0;

  calculateSpecificity( classifier );

  return classifier;
}


int matchClassifiers( unsigned char *condition )
{
  int c;
  int i;
  double bidSum = 0.0;
  int matchFound = 0;

  /* Match condition with the list of classifiers */
  for (c = 0 ; c < MAX_CLASSIFIERS ; c++) {

    list[c].match = 0;

    for (i = 0 ; i < MAX_COND_WIDTH ; i++) {

      if (!match(list[c].condition[i], condition[i])) break;

    }

    if (i == MAX_COND_WIDTH) {
      list[c].match = 1;
      list[c].bid = RISK_FACTOR * list[c].strength * list[c].specificity;
      bidSum += list[c].bid;
      matchFound = 1;
    }

  }

  if (matchFound) {

    int tries = MAX_CLASSIFIERS;

    c = getRand(MAX_CLASSIFIERS);
    while (1) {

      if (list[c].match) {
        if ( (list[c].bid / bidSum) > getSRand()) break;
        if ( --tries == 0) break;
      }

      if (++c >= MAX_CLASSIFIERS) c = 0;

    }

  } else {

    if (c == MAX_CLASSIFIERS) {
      c = createCoveringClassifier( -1, condition );
    }

  }

  return c;
}


void sendEffectors( int classifier )
{
  int i, x, y;
  int act=0;

  for (i = MAX_ACTION_WIDTH-1 ; i >= 0 ; i--) {

    if ( list[classifier].action[i] == '1') {

      act |= (1 << ((MAX_ACTION_WIDTH-1) - i));

    }


  }

  y = agent_y + dir[act].loc_y;
  x = agent_x + dir[act].loc_x;

  if (world[y][x] == ' ') {

    world[agent_y][agent_x] = ' ';

    /* Classifier led to a successful move, increase the strength
     * of this classifer and move the agent to its new position.
     */
    agent_y = y;
    agent_x = x;

    list[classifier].strength += REWARD;

    list[classifier].episode = 1;

    world[agent_y][agent_x] = '@';

  } else {

    /* Classifier was bad, replace it with a new one.  */
    createCoveringClassifier( classifier, list[classifier].condition );

  }

  return;
}


void initWorld( void )
{
  int i, x, y;

  memset( (char *)world, ' ', sizeof(world) );

  for (x = 0 ; x < MAX_WORLD_X ; x++) {
    world[0][x] = '*';
    world[MAX_WORLD_Y-1][x] = '*';
  }

  for (y = 0 ; y < MAX_WORLD_Y ; y++) {
    world[y][0] = '*';
    world[y][MAX_WORLD_X-1] = '*';
  }

  i = 0;
  while (i < MAX_OBSTACLES) {

    y = getRand(MAX_WORLD_Y);
    x = getRand(MAX_WORLD_X);

    if (world[y][x] != '*') {
      world[y][x] = '*';
      i++;
    }

  }

  return;
}


void initClassifier( int classifier )
{
  int cond, act;

  for (cond = 0 ; cond < MAX_COND_WIDTH ; cond++) {
    list[classifier].condition[cond] = getRandomCharacter();
  }

  for (act = 0 ; act < MAX_ACTION_WIDTH ; act++) {
    list[classifier].action[act] = 0x30 + getRand(2);
  }

  list[classifier].strength = 1.0;
  list[classifier].episode = 0;

  calculateSpecificity( classifier );

  return;
}


void initAgent( void )
{
  int c, i;

  /* Create the classifier population */
  for (c = 0 ; c < MAX_CLASSIFIERS ; c++) {
    initClassifier( c );
  }

  /* Place the agent in a comfortable spot in the world */
  while (1) {

    agent_y = 1+getRand(MAX_WORLD_Y-2);
    agent_x = 1+getRand(MAX_WORLD_X-2);

    if (world[agent_y][agent_x] == '*') continue;

    for (i = 0 ; i < MAX_DIR ; i++) {

      int y = agent_y + dir[i].loc_y;
      int x = agent_x + dir[i].loc_x;

      if (world[y][x] == '*') break;

    }

    /* If agent has no obstacles around it, choose this location */
    if (i == MAX_DIR) break;

  }

  world[agent_y][agent_x] = '@';

  return;
}


void printAgent( void )
{
  int c, cond, act;

  printf("\nClassifier List:\n");
  for (c = 0 ; c < MAX_CLASSIFIERS ; c++) {

    printf( "C%02d ", c );

    for (cond = 0 ; cond < MAX_COND_WIDTH ; cond++) {
      printf("%c", list[c].condition[cond]);
    }
    printf(" : ");

    for (act = 0 ; act < MAX_ACTION_WIDTH ; act++) {
      printf("%c", list[c].action[act]);
    }

    printf("  Strength %g\n", list[c].strength);

  }

  return;
}


void printWorld( void )
{
  int y, x;

  printf("World:\n");

  for (y = 0 ; y < MAX_WORLD_Y ; y++) {

    printf("\t");

    for (x = 0 ; x < MAX_WORLD_X ; x++) {

      printf("%c ", world[y][x]);

    }

    printf("\n");

  }

  return;
}


int main()
{
  int t, i, c;
  unsigned char detectors[MAX_COND_WIDTH];

  srand( time(NULL) );

  initWorld();
  initAgent();

  for (t = 0 ; t < 1000 ; t++) {

    readDetectors( detectors );

    c = matchClassifiers( detectors );

    sendEffectors( c );

    system("clear");
    printWorld();

    /* If we've completed an episode */
    if ( (t % MAX_EPISODE) == 0 ) {

      /* Tax the last classifer */
      list[c].strength -= list[c].bid;
      if (list[c].strength < 0.0) list[c].strength = 0.0;

      /* Reward classifiers that supported the episode */
      for (i = 0 ; i < MAX_CLASSIFIERS ; i++) {
        if ((list[i].episode) && (i != c)) {
          list[i].strength += ( list[c].bid / (double)MAX_EPISODE );
          list[i].episode = 0;
        }
      }

    }

  }

  printAgent();

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
