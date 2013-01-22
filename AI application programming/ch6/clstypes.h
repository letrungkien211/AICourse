/*
 *  Classifer Systems Types and Symbolics
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdlib.h>

#define MAX_COND_WIDTH		4
#define MAX_ACTION_WIDTH	2
#define MAX_CLASSIFIERS		32  // More Classifiers, more options
#define MAX_EPISODE		10

typedef struct {

  unsigned char condition[MAX_COND_WIDTH];
  unsigned char action[MAX_ACTION_WIDTH];
  double        strength;
  double        specificity;
  double        bid;
  int           match;
  int           episode;

} classifier_t;

#define MAX_WORLD_X		20
#define MAX_WORLD_Y		20

#define MAX_OBSTACLES		100

#define RISK_FACTOR		((double)0.5)

#define getRand(x)		(int)((double)(x)*rand()/(RAND_MAX+1.0))
#define getSRand()      	((float)rand() / (float)RAND_MAX)

#define MAX_DIR			4

typedef struct {
  int loc_y;
  int loc_x;
} dir_t;

#define REWARD		1.0


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
