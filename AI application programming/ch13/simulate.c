/*
 *  Fuzzy Logic Simulation Functions
 *
 *  mtj@cogitollc.com
 *
 */

#include <math.h>
#include <stdlib.h>
#include "mtemp.h"

#define getSRand()	((float)rand() / (float)RAND_MAX)
#define getRand(x)	(int)((float)(x)*rand()/(RAND_MAX+1.0))

float voltage = 20.0;
float temperature = 12.0;
float timer = 0.0;

#define MAX_LOADS	5
const float load[MAX_LOADS]={0.02, 0.04, 0.06, 0.08, 0.1};

static int curLoad = 0;


double charge( int t )
{
  double result;

  result = sin( (double)t/100.0 );

  if (result < 0.0) result = 0.0;

  return result;
}


int simulate( void )
{
  extern int chargeMode;
  static int t=0;

  /* First, update the loading if necessary */
  if (getSRand() < 0.02) {
    curLoad = getRand( MAX_LOADS );
  }

  /* Affect the current battery voltage given the load */
  voltage -= load[curLoad];

  /* Next, update the battery voltage given input charge */
  if (chargeMode == FAST_CHARGE) {
    voltage += (charge(t) * sqrt(timer));
  } else {
    voltage += ((charge(t) * sqrt(timer)) / 10.0 );
  }

  if      (voltage < 0.0)  voltage = 0.0;
  else if (voltage > 35.0) voltage = 35.0;

  /* Update the temperature */
  if (chargeMode == FAST_CHARGE) {
    if        (voltage > 25) {
      temperature += ((load[curLoad] * (sqrt(timer)/25.0)) * 10.0);
    } else if (voltage > 15) {
      temperature += ((load[curLoad] * (sqrt(timer)/20.0)) * 10.0);
    } else {
      temperature += ((load[curLoad] * (sqrt(timer)/15.0)) * 10.0);
    }
  } else {
    if        (temperature > 20.0) {
      temperature -= ((load[curLoad] * (sqrt(timer)/20.0)) * 10.0);
    } else {
      temperature -= ((load[curLoad] * (sqrt(timer)/100.0)) * 10.0);
    }
  }

  if      (temperature < 0.0) temperature = 0.0;
  else if (temperature > 40.0) temperature = 40.0;

  t++;

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
