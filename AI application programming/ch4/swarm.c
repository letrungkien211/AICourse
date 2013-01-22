/*
 *  Particle Swarm Optimization swarm functions
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include "pso.h"

void moveParticle( int i )
{
  extern double c1, c2, dt;

  extern particle_t particles[], pbest[];
  extern particle_t gbest;
  extern double checkFitness( double, double );

  /* Update the position of the particle */
  particles[i].position.x += (particles[i].velocity.x * dt);
  particles[i].position.y += (particles[i].velocity.y * dt);

  /* Since we're interested only in the -2PI-2PI range, we'll
   * calculate fitness only over this range.
   */
  if ( (particles[i].position.x > (-2.0 * PI)) && 
       (particles[i].position.x < ( 2.0 * PI)) &&
       (particles[i].position.y > (-2.0 * PI)) &&
       (particles[i].position.y < ( 2.0 * PI)) ) {

    particles[i].fitness = 
        checkFitness( particles[i].position.x, particles[i].position.y );

  }

  /* Write the particle's position and fitness to the trace file */
  fprintf( particles[i].fp, "%lg, %lg, %lg\n", 
           particles[i].position.x, particles[i].position.y, 
           particles[i].fitness );

  /* Update the velocity vector of the particle */
  particles[i].velocity.x +=
    ( (c1 * getSRand() * (gbest.position.x - particles[i].position.x)) + 
      (c2 * getSRand() * (pbest[i].position.x - particles[i].position.x)) );

  particles[i].velocity.y +=
    ( (c1 * getSRand() * (gbest.position.y - particles[i].position.y)) + 
      (c2 * getSRand() * (pbest[i].position.y - particles[i].position.y)) );

  return;
}


void moveSwarm( void )
{
  int j;

  extern particle_t particles[], pbest[];
  extern void storePbest( int );

  /* Move each particle in the swarm */
  for ( j = 0 ; j < MAX_PARTICLES ; j++ ) {

    moveParticle( j );

    if ( particles[j].fitness > pbest[j].fitness ) storePbest( j );

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
