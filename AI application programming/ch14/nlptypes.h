/*
 *  Natural Language Processing Types and Symbolics
 *
 *  mtj@cogitollc.com
 *
 */


/* Grammar Symbols */
#define DET		1	/* Determiner Symbol */
#define NOUN		2	/* Noun              */
#define VERB		3	/* Verb Symbol       */
#define ADJECTIVE	4       /* Adjective Symbol  */
#define EOS		5	/* End of Sentence   */

/* Maximum number of words possible in the lexicon */
#define MAX_WORDS	30

/* word/type typedef */
typedef struct {
  char *word;
  int  type;
} word_t;

/* Lexicon type */
typedef struct {
  word_t words[MAX_WORDS];
} lexicon_t;


/* Finite State Machine Parser States */
#define S_STATE			0
#define NP_STATE		1
#define NP_DET_STATE		2
#define NP_NOUN_STATE		3
#define VP_STATE		4
#define VP_VERB_STATE		5
#define VP_NP_STATE		6
#define VP_NP_DET_STATE		7
#define VP_NP_ADJ_STATE		8
#define VP_NP_NOUN_STATE	9
#define EOS_STATE		10


/* Simple Knowledge Frame Type */
typedef struct {
  char *actor;
  char *action;
  char *modifier;
  char *object;
} frame_t;


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
