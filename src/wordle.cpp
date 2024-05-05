#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "wordle.h"

namespace Wordle{

int gen_random_word_index(){
  srand(time(0));
  return (rand() % WORDS_COUNT)*(WORD_LENGTH+1);
}

bool load_words(char *wordsOut){
  FILE *wordsFile = fopen("words", "r");
  if (wordsFile) {
    fgets(wordsOut, WORDS_COUNT*(WORD_LENGTH+1), wordsFile);
    fclose(wordsFile);
    gen_first_letters_table(wordsOut);
    return true;
  }
  return false;
}

void gen_first_letters_table(const char *words){
  firstLettersIndex[0] = 0;

  for (int wordIndex = 0, cacheIndex = 1; wordIndex <= WORDS_COUNT; wordIndex++){
    int currentLetterIndex = wordIndex*(WORD_LENGTH+1);
    int nextLetterIndex = (wordIndex+1) * (WORD_LENGTH+1);
    const char currentFirstLetter = words[currentLetterIndex];
    const char nextFirstLetter = words[nextLetterIndex];
    if (currentFirstLetter!= nextFirstLetter)
      firstLettersIndex[cacheIndex++] = nextLetterIndex;
  }

  firstLettersIndex[26] = WORDS_COUNT*(WORD_LENGTH+1);
}

bool verify_input(const char *input, const char *words){
  char firstLetter = input[0];

  int firstLetterBeginIndex = firstLettersIndex[firstLetter - 'A'],
      firstLetterEndIndex  = firstLettersIndex[firstLetter - 'A' + 1];

  for (int wordIndex = firstLetterBeginIndex; wordIndex <= firstLetterEndIndex; wordIndex += WORD_LENGTH+1){
    const char *currentWord = words + wordIndex;
    if(!strncmp(currentWord, input, WORD_LENGTH))
      return true;
  }

  return false;
}

GameState game_state(Color colors[WORD_LENGTH], int numGuesses){
  static Color winingCondition[WORD_LENGTH] = {W_GREEN, W_GREEN, W_GREEN, W_GREEN, W_GREEN};
  if(numGuesses < MAX_GUESSES){
    if(!memcmp(colors, winingCondition, sizeof(Color)*WORD_LENGTH))
      return WIN;

    return CONTINUE;
  }
  return  LOST;
}

void game_logic(const char *guess, const char *chosen, Color colorsOut[WORD_LENGTH]){
  char copy[WORD_LENGTH+1];
  strncpy(copy, chosen, WORD_LENGTH);

  for (int x = 0; x < WORD_LENGTH; x++) {
    char ch = guess[x];
    if (copy[x] == ch) {
      colorsOut[x] = W_GREEN;
      copy[x] = '#';
    }
  }

  for (int x = 0; x < WORD_LENGTH; x++)
    if(colorsOut[x] != W_GREEN)
      for (int y = 0; y < WORD_LENGTH; y++)
        if (copy[y] == guess[x]) {
          colorsOut[x] = W_YELLOW;
          copy[y] = '#';
        }
}

}
