/*
 *  Backpropagation Algorithm -- Simple MLP Batch Training Example
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double inp_0, inp_1;
double hid_0, hid_1;
double out;
double target;

double w_hid0inp0, w_hid0inp1, w_hid1inp0, w_hid1inp1;
double w_outh0, w_outh1;
double h0_bias, h1_bias, out_bias;
double bw_hid0inp0=0.0, bw_hid0inp1=0.0, bw_hid1inp0=0.0, bw_hid1inp1=0.0;
double bw_outh0=0.0, bw_outh1=0.0;
double bh0_bias=0.0, bh1_bias=0.0, bout_bias=0.0;

double rho = 3.0;
#define NUM_EPOCHS	10000

#define getRand(x)	(int)((float)(x)*rand()/(RAND_MAX+1.0))
#define RAND_WEIGHT	(((double)rand() / (double)RAND_MAX))
#define sqr(x)		((x) * (x))

#define HIGH	0.9
#define LOW	0.1
#define MID	(LOW + ((HIGH-LOW)/2.0))

double sigmoid( double val )
{
  return (1.0 / (1.0 + exp(-val)));
}

double sigmoidDerivative( double val )
{
  return (val * (1.0 - val));
}

void initializeNetwork( void )
{
  w_hid0inp0 = RAND_WEIGHT; w_hid0inp1 = RAND_WEIGHT;
  w_hid1inp0 = RAND_WEIGHT; w_hid1inp1 = RAND_WEIGHT;
  w_outh0 = RAND_WEIGHT;    w_outh1 = RAND_WEIGHT;

  h0_bias = RAND_WEIGHT; h1_bias = RAND_WEIGHT; out_bias = RAND_WEIGHT;

  return;
}

void feedforward( void )
{
  /* First, calculate hidden node 0 */
  hid_0 = sigmoid( (inp_0 * w_hid0inp0) + (inp_1 * w_hid0inp1) + h0_bias );

  /* Next, calculate hidden node 1 */
  hid_1 = sigmoid( (inp_0 * w_hid1inp0) + (inp_1 * w_hid1inp1) + h1_bias );

  /* Finally, calculate the output node */
  out = sigmoid(((hid_0 * w_outh0) + (hid_1 * w_outh1) + out_bias));

  return;
}

void backpropagate( void )
{
  double out_err, hid0_err, hid1_err;

  /* Calculate output layer error */
  out_err = (target - out) * sigmoidDerivative(out);  

  /* Calculate the hidden layer error */
  hid0_err = (out_err * w_outh0) * sigmoidDerivative(hid_0);
  hid1_err = (out_err * w_outh1) * sigmoidDerivative(hid_1);

  /* Updated the output layer weights (and bias) */
  bw_outh0  += (rho * out_err * hid_0);
  bw_outh1  += (rho * out_err * hid_1);
  bout_bias += (rho * out_err);

  /* Update weights for hidden node 0 and bias */
  bw_hid0inp0 += ((rho * hid0_err) * inp_0);
  bw_hid0inp1 += ((rho * hid0_err) * inp_1);
  bh0_bias += (rho * hid0_err);

  /* Update weights for hidden node 1 and bias */
  bw_hid1inp0 += ((rho * hid1_err) * inp_0);
  bw_hid1inp1 += ((rho * hid1_err) * inp_1);
  bh1_bias += (rho * hid1_err);

  return;
}

void batchUpdate( void )
{
  w_hid0inp0 += bw_hid0inp0; w_hid0inp1 += bw_hid0inp1;
  w_hid1inp0 += bw_hid1inp0; w_hid1inp1 += bw_hid1inp1;
  w_outh0 += bw_outh0; w_outh1 += bw_outh1;
  h0_bias += bh0_bias; h1_bias += bh1_bias;
  out_bias += bout_bias;

  bw_hid0inp0 = bw_hid0inp1 = bw_hid1inp0 = bw_hid1inp1 = 0.0;
  bw_outh0 = bw_outh1 = 0.0;
  bh0_bias = bh1_bias = bout_bias = 0.0;

  return;
}

typedef struct {
  double inp0;
  double inp1;
  double out;
} dataset_t;

/* Xor dataset */
dataset_t dataset[4] = {
  {  LOW,  LOW,  LOW},
  {  LOW, HIGH, HIGH},
  { HIGH,  LOW, HIGH},
  { HIGH, HIGH,  LOW}
};

int main()
{
  double error=0.0;
  int epoch, i; 
  int count = 0;

  srand(time(NULL));

  initializeNetwork();

  for (epoch = 0 ; epoch < NUM_EPOCHS ; epoch++) {

//    i = getRand(4);
    i = count;
    inp_0 = dataset[i].inp0; 
    inp_1 = dataset[i].inp1;
    target = dataset[i].out;

    feedforward();
    backpropagate();

    error += (sqr(target - out));

    if (++count == 4) {
      printf("%lg\n", (0.25*error));
      count = 0;
      error = 0.0;
      batchUpdate();
    }

  }

  printf("Testing...\n");

  /* Test the network */
  for (i = 0 ; i < 4 ; i++) {

    inp_0 = dataset[i].inp0;
    inp_1 = dataset[i].inp1;
    target = dataset[i].out;
    feedforward();

    printf("test %lg/%lg = %lg\n", inp_0, inp_1, out);

    out = (out >= MID) ? HIGH : LOW;

    if (out == target) printf("Success\n");
    else printf("Failed\n");

  }

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
 *       its contributors may be used to endorse or promote 
 *       products derived from this software without specific 
 *       prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CHARLES RIVER MEDIA AND CONTRIBUTORS 
 * 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CHARLES
 * RIVER MEDIA OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
*/
