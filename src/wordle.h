#pragma once

namespace Wordle{

  #define WORDS_COUNT 12947
  #define WORD_LENGTH 5
  #define MAX_GUESSES 6

  static int firstLettersIndex[27];

  enum Color{ W_GRAY, W_YELLOW, W_GREEN };
  enum GameState{ CONTINUE, WIN, LOST };

  int gen_random_word_index();
  bool load_words(char *wordsOut);
  bool verify_input(const char *input, const char *words);
  GameState game_state(Color colors[5], int numGuesses);
  void game_logic(const char *guess, const char *chosen, Color colorsOut[5]);
  void gen_first_letters_table(const char *words);

}
