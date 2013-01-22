/*
 *  Natural Language Processing implementation
 *
 *  mtj@cogitollc.com
 *
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "nlptypes.h"

static lexicon_t lexicon;
static char *workingString;
static int windex;

frame_t frame;

void addWord( int type, char *word )
{
  int i=0;

  while (1) {

    if (lexicon.words[i].word == (char *)NULL) {
      lexicon.words[i].type = type;
      lexicon.words[i].word = word;
      return;
    } else {
      if (++i == MAX_WORDS) break;
    }

  }

  /* Protect against overflow */
  assert(0);

  return;
}


void findWord( word_t *symbol )
{
  int i;

  symbol->type = EOS;

  /* Find the word, and return its type */
  for (i = 0 ; i < MAX_WORDS ; i++) {
    if (lexicon.words[i].word != (char *)NULL) {
      if (!strncmp(lexicon.words[i].word, 
                    symbol->word, strlen(symbol->word))) {
        symbol->type = lexicon.words[i].type;
        return;
      }
    }
  }

  return;
}


void init( void )
{
  /*
   * Clear the lexicon structure and then add the words and 
   * their types 
   */

  memset( &lexicon, 0, sizeof(lexicon_t) );

  addWord( DET, "the" ); addWord( DET, "a" );

  addWord( NOUN, "man" ); addWord( NOUN, "house" );
  addWord( NOUN, "dog" ); addWord( NOUN, "fruit" );
  addWord( NOUN, "car" ); addWord( NOUN, "cat" );

  addWord( VERB, "washed" );  addWord( VERB, "ate" );
  addWord( VERB, "drove" );   addWord( VERB, "chased" );
  addWord( VERB, "painted" ); addWord( VERB, "marked" );

  addWord( ADJECTIVE, "blue" );     addWord( ADJECTIVE, "angry" );
  addWord( ADJECTIVE, "dirty" );    addWord( ADJECTIVE, "quiet" );
  addWord( ADJECTIVE, "confused" ); addWord( ADJECTIVE, "noisy" );

  return;
}


void initSentence( char *sentence )
{
  int len;

  len = strlen(sentence);
  workingString = malloc( len+1 );
  strncpy( workingString, sentence, len );
  workingString[len] = 0;
  windex = 0;

  frame.actor = NULL;
  frame.action = NULL;
  frame.modifier = NULL;
  frame.object = NULL;

  return;
}


void destroySentence( void )
{
  free( workingString );
  return;
}


void getWord( word_t *symbol )
{

  /* Eat the whitespace */
  while (workingString[windex] == ' ') windex++;

  /* Found the end of the string, return EOS */
  if (workingString[windex] == 0) {
    symbol->type = EOS;
    return;
  }

  /* Store the word's address */
  symbol->word = &workingString[windex];

  /* Walk through word, looking for the end */
  while ((workingString[windex] != ' ') &&
         (workingString[windex] !=   0)) windex++;

  /* Null terminate the word (in the string) */
  if (workingString[windex] == ' ') {
    workingString[windex++] = 0;
  }

  /* Find the word, and get it's type */
  findWord(symbol);

  return;
}


char *strtype( int type )
{
  switch(type) {

    case DET:       return("DET");       break;
    case NOUN:      return("NOUN");      break;
    case VERB:      return("VERB");      break;
    case ADJECTIVE: return("ADJECTIVE"); break;
    case EOS:       return("EOS");       break;

  }

  return ("BAD TYPE");
}


void emitFrame( void )
{
  printf("\n");
  if (frame.actor) printf("Actor: %s\n", frame.actor);
  if (frame.action) printf("  Action: %s\n", frame.action);
  if (frame.object) {
    printf("    Object: ");
    if (frame.modifier) printf("%s ", frame.modifier);
    printf("%s\n", frame.object);
  }
  printf("\n\n");

  return;
}


/* Deterministic Grammar Parser */

void parseSentence( void )
{
  int  state = S_STATE;
  word_t symbol;

  while (state != EOS_STATE) {

    switch( state ) {

      case S_STATE:
        printf("(S ");
        state = NP_STATE;
        break;

      case NP_STATE:
        printf("(NP ");
        state = NP_DET_STATE;
        break;

      case NP_DET_STATE:
        getWord( &symbol );
        if (symbol.type == DET) {
          printf("[DET %s] ", symbol.word);
          getWord( &symbol );
        }
        state = NP_NOUN_STATE;
        break;

      case NP_NOUN_STATE:
        if (symbol.type == NOUN) {
          printf("(NOUN %s) )", symbol.word);
          frame.actor = symbol.word;
          state = VP_STATE;
          getWord( &symbol );
        } else {
          printf("!ERROR! (type=%s)\n", strtype(symbol.type));
          state = EOS_STATE;
        }
        break;

      case VP_STATE:
        if (symbol.type == EOS) {
          printf(" )\n");
          state = EOS_STATE;
        } else {
          printf("\n\t(VP ");
          state = VP_VERB_STATE;
        }
        break;

      case VP_VERB_STATE:
        if (symbol.type == VERB) {
          printf("(VERB %s) ", symbol.word);
          frame.action = symbol.word;
          state = VP_NP_STATE;
          getWord( &symbol );
        } else {
          printf("!ERROR! (type=%s)\n", strtype(symbol.type));
          state = EOS_STATE;
        }
        break;

      case VP_NP_STATE:
        if (symbol.type == EOS) {
          printf(" ) )\n");
          state = EOS_STATE;
        } else {
          printf("\n\t\t(VP_NP ");
          state = VP_NP_DET_STATE;
        }
        break;

      case VP_NP_DET_STATE:
        if (symbol.type == DET) {
          printf("[DET %s] ", symbol.word);
          getWord( &symbol );
        }
        state = VP_NP_ADJ_STATE;
        break;

      case VP_NP_ADJ_STATE:
        if (symbol.type == ADJECTIVE) {
          printf("[ADJ %s] ", symbol.word);
          frame.modifier = symbol.word;
          getWord( &symbol );
        }
        state = VP_NP_NOUN_STATE;
        break;

      case VP_NP_NOUN_STATE:
        if (symbol.type == NOUN) {
          printf("(NOUN %s) ) ) )\n", symbol.word);
          frame.object = symbol.word;
          state = EOS_STATE;
        } else {
          printf("!ERROR! (type=%s)\n", strtype(symbol.type));
          state = EOS_STATE;
        }
        break;

    }

  }

  return;
}


int main()
{
  init();

  initSentence( "the man washed the blue car" );
  parseSentence();
  emitFrame();
  destroySentence();

  initSentence( "a dog chased a dirty cat" );
  parseSentence();
  emitFrame();
  destroySentence();

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
