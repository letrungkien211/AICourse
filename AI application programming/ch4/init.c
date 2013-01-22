/*
 *  Particle Swarm Optimization initialization functions
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "pso.h"

/* Array of particles within the solution space */
particle_t particles[MAX_PARTICLES];
particle_t pbest[MAX_PARTICLES];

/* Global Best particle */
particle_t gbest;


void storePbest( int index )
{
  /* Save the particle to its personal best */
  memcpy( (void *)&pbest[index], (void *)&particles[index], 
           sizeof(particle_t) );

  /* If this pbest is better than the gbest, save it */
  if (pbest[index].fitness > gbest.fitness) {
    gbest.position.x = pbest[index].position.x;
    gbest.position.y = pbest[index].position.y;
    gbest.fitness    = pbest[index].fitness;
  }

  return;
}


#define MAX_FILENAME	80

void initPopulation( void )
{
  int i;
  char filename[MAX_FILENAME+1];

  extern double checkFitness( double, double );

  gbest.fitness = (double)-9999.0;  

  for ( i = 0 ; i < MAX_PARTICLES ; i++ ) {

    /* Initialize the output file pointer */
    sprintf( filename, "particle%02d.txt", i );
    particles[i].fp = fopen( filename, "w" );

    /* Initialize a particle's starting point and fitness*/
    particles[i].position.x = getPoint();
    particles[i].position.y = getPoint();
    particles[i].fitness = 
      checkFitness( particles[i].position.x, particles[i].position.y );

    /* Initialize the particle's dimensional velocity */
    particles[i].velocity.x = (getSRand() / (double)10.0);
    particles[i].velocity.y = (getSRand() / (double)10.0);

    /* Since this is the only fitness value, store this as pbest */
    storePbest( i );

    /* Store this position to the particles trace file */
    fprintf( particles[i].fp, "%lg, %lg, %lg\n",
             particles[i].position.x, particles[i].position.y, 
             particles[i].fitness );

  }

  return;
}


void closePopulation( void )
{
  int i;

  for ( i = 0 ; i < MAX_PARTICLES ; i++ ) {

    fclose( particles[i].fp );

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
