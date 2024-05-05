#include <iostream>
#include <cstring>

#include "wordle.h"

int main(){

  char words[WORDS_COUNT*(WORD_LENGTH+1)];
  if (!Wordle::load_words(words)) {
    std::cerr << "\033[31mCould not open the words file!\033[0m" << std::endl;
    return 0;
  }

  char *chosen = words + Wordle::gen_random_word_index();
  int numGuesses = 0;

  std::cout << "Start entering your guesses:" << std::endl;
  while(true){
    char guess[64];
    scanf("%s", guess);

    if(strlen(guess) != WORD_LENGTH){
      std::cerr << "\033[31mYour guess should be 5 characters long!\033[0m" << std::endl;
      continue;
    }

    for(int c=0;c<WORD_LENGTH;c++)
      if(guess[c] >= 'a')
	guess[c] -= 'a' - 'A';
    
    if(!Wordle::verify_input(guess, words)){
      std::cerr << "\033[31mYour guess is not a valid word!\033[0m" << std::endl;
      continue;
    }

    Wordle::Color colors[] = {Wordle::W_GRAY, Wordle::W_GRAY, Wordle::W_GRAY, Wordle::W_GRAY, Wordle::W_GRAY};
    Wordle::game_logic(guess, chosen, colors);
    numGuesses++;

    //'pretty' print output
    for (int x = 0; x < WORD_LENGTH; x++) {
      switch (colors[x]){
      case Wordle::W_GRAY:   std::cout << guess[x]; break;
      case Wordle::W_YELLOW: std::cout << "\033[33m" << guess[x] << "\033[0m"; break;
      case Wordle::W_GREEN:  std::cout << "\033[32m" << guess[x] << "\033[0m"; break;
      }
    }
    std::cout << std::endl;

    Wordle::GameState state = Wordle::game_state(colors, numGuesses);
    switch(state){
    case Wordle::WIN: std::cout << "\n\033[32mCongrats, you win!\nTry again? [y/n]: \033[0m" << std::endl; break;

    case Wordle::LOST:{
      //TODO: come with something instead of copying string
      char copy[WORD_LENGTH + 1];
      strncpy(copy, chosen, WORD_LENGTH);
      copy[WORD_LENGTH] = '\0';
      std::cout << "\n\033[31mYou Lost!\nIt Was: \033[0m" << copy << "\n\033[31mTry again? [y/n]: \033[0m" << std::endl;
    } break;

    case Wordle::CONTINUE: continue;
    }

    char yesNo[64];
    scanf("%s", yesNo);
    if(yesNo[0] == 'y'){
      chosen = words + Wordle::gen_random_word_index();
      numGuesses = 0;
      continue;
    }
    else break;
  }
  
  return 0;
}
