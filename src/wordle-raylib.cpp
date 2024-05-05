#include <iostream>

#include "wordle.h"

namespace Raylib {
#include <raylib.h>
} //bad practice i guess, thank you raylib very cool :(

#define GUESS_RECT_WIDTH (WINDOW_WIDTH/10)
#define GUESS_RECT_HEIGHT (WINDOW_HEIGHT/10)
#define GUESS_RECT_PADDING_WIDTH (GUESS_RECT_WIDTH/16)
#define GUESS_RECT_PADDING_HEIGHT (GUESS_RECT_HEIGHT/16)

#define KEYBOARD_WIDTH 9
#define KEYBOARD_HEIGHT 3 
#define KEYBOARD_RECT_HEIGHT (WINDOW_HEIGHT/10)
#define KEYBOARD_RECT_WIDTH (WINDOW_WIDTH/12)
#define KEYBOARD_RECT_PADDING_WIDTH (KEYBOARD_RECT_WIDTH/16)
#define KEYBOARD_RECT_PADDING_HEIGHT (KEYBOARD_RECT_HEIGHT/16)

enum MsgType{ CONTINUE_MSG, WIN_MSG, LOST_MSG, INVALID_INPUT_MSG };
const MsgType gameStateToMsg[] = {
  [Wordle::GameState::CONTINUE] = CONTINUE_MSG,
  [Wordle::GameState::WIN] = WIN_MSG,
  [Wordle::GameState::LOST] = LOST_MSG
};

const char *msgs[] = {
  [MsgType::CONTINUE_MSG] = "",
  [MsgType::WIN_MSG] = "You Win!",
  [MsgType::LOST_MSG] = "You Lost!",
  [MsgType::INVALID_INPUT_MSG] = "Your word is not valid!"
};

const Raylib::Color wordleColorToRaylibColor[] = {
  [Wordle::Color::W_GRAY] = Raylib::GRAY,
  [Wordle::Color::W_YELLOW] = Raylib::YELLOW,
  [Wordle::Color::W_GREEN] = Raylib::GREEN
};

const char keys[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-" ;

int main(){
  MsgType msgIndex = CONTINUE_MSG;
  char keysClicked[WORD_LENGTH*MAX_GUESSES];
  int currentGuessRectIndex = 0;
  Raylib::Color guessRectColors[WORD_LENGTH*MAX_GUESSES];
  for(int ndx=0;ndx<WORD_LENGTH*MAX_GUESSES;ndx++) guessRectColors[ndx] = Raylib::GRAY;
  int numGuesses = 0;

  char words[WORDS_COUNT*(WORD_LENGTH+1)];
  if(!Wordle::load_words(words)){
    std::cerr << "\033[31m[DEBUG]: Could not open the words file!\033[0m" << std::endl;
    return 1;
  }

  const char *chosen = words + Wordle::gen_random_word_index();

  Raylib::InitWindow(800, 800, "Wordle");
  Raylib::SetTargetFPS(60);
  Raylib::SetWindowState(Raylib::FLAG_WINDOW_RESIZABLE);
  while(!Raylib::WindowShouldClose()){

    int WINDOW_WIDTH = Raylib::GetScreenWidth();
    int WINDOW_HEIGHT = Raylib::GetScreenHeight();

    bool isFinishedRow = ((currentGuessRectIndex == ((numGuesses+1)*WORD_LENGTH)) && currentGuessRectIndex != 0);

    Raylib::BeginDrawing();
    Raylib::ClearBackground({0x18,0x18,0x18,0xAA});

    //draw guesses 
    for(int y = 0; y < MAX_GUESSES; y++)
      for(int x = 0; x < WORD_LENGTH; x++){
	int rectCoordX = x * GUESS_RECT_WIDTH + (WINDOW_WIDTH - GUESS_RECT_WIDTH * WORD_LENGTH) / 2; //WINDOW_WIDTH = 2x + GUESS_RECT_WIDTH*WORD_LENGTH
	int rectCoordY = (y*GUESS_RECT_HEIGHT + GUESS_RECT_PADDING_HEIGHT);
	int rectWidth = GUESS_RECT_WIDTH - GUESS_RECT_PADDING_WIDTH;
	int rectHeight = GUESS_RECT_HEIGHT - GUESS_RECT_PADDING_HEIGHT;
	Raylib::DrawRectangle(rectCoordX, rectCoordY, rectWidth, rectHeight, guessRectColors[y*WORD_LENGTH + x]);
      }

    //draw keyboard
    for(int y = 0; y < KEYBOARD_HEIGHT; y++)
      for(int x = 0; x < KEYBOARD_WIDTH; x++){
	int rectCoordX = x * KEYBOARD_RECT_WIDTH + (WINDOW_WIDTH - KEYBOARD_RECT_WIDTH * KEYBOARD_WIDTH)/2;
	int rectCoordY = WINDOW_HEIGHT - KEYBOARD_HEIGHT * KEYBOARD_RECT_HEIGHT + y * KEYBOARD_RECT_HEIGHT;
	int rectWidth = KEYBOARD_RECT_WIDTH - KEYBOARD_RECT_PADDING_WIDTH;
	int rectHeight = KEYBOARD_RECT_HEIGHT - KEYBOARD_RECT_PADDING_HEIGHT;
	Raylib::DrawRectangle(rectCoordX, rectCoordY, rectWidth, rectHeight, Raylib::BLACK);

	int fontSize = 18;
	int centerRectX = rectCoordX + KEYBOARD_RECT_WIDTH/2 - fontSize/2;
	int centerRectY = rectCoordY + KEYBOARD_RECT_HEIGHT/2 - fontSize/2;
	char currentCharacter = keys[y * KEYBOARD_WIDTH + x];
	Raylib::DrawTextCodepoint(Raylib::GetFontDefault(), currentCharacter, {(float)centerRectX, (float)centerRectY}, fontSize, Raylib::WHITE);

	//handle mouse clicks
	int mouseX = Raylib::GetMouseX();
	int mouseY = Raylib::GetMouseY();
	bool isWithinRegion = (mouseX > rectCoordX) && (mouseX < (rectCoordX + rectWidth)) && (mouseY > rectCoordY) && (mouseY < (rectCoordY + rectHeight));

	if(isWithinRegion && IsMouseButtonReleased(Raylib::MOUSE_BUTTON_LEFT)){
	  bool isBackspaceClicked = (y == KEYBOARD_HEIGHT-1) && (x == KEYBOARD_WIDTH-1);
	  if(isBackspaceClicked){
	    int currentRow = numGuesses*WORD_LENGTH;
	    if(currentGuessRectIndex > currentRow)
	      keysClicked[--currentGuessRectIndex] = ' ';
	  }
	  else if(!isFinishedRow && (currentGuessRectIndex < WORD_LENGTH*MAX_GUESSES))
	    keysClicked[currentGuessRectIndex++] = currentCharacter;
	}
      }

    //draw selected character in the current guess rectangle
    for(int keyClickedNdx=0; keyClickedNdx < currentGuessRectIndex; keyClickedNdx++){
      int currentGuessRectX = keyClickedNdx % WORD_LENGTH; 
      int currentGuessRectY = keyClickedNdx / WORD_LENGTH;
      int rectCoordX =  currentGuessRectX * GUESS_RECT_WIDTH + (WINDOW_WIDTH - GUESS_RECT_WIDTH * WORD_LENGTH)/2;
      int rectCoordY = currentGuessRectY * GUESS_RECT_HEIGHT + GUESS_RECT_PADDING_HEIGHT;
      int fontSize = 28;
      int centerRectX = rectCoordX + GUESS_RECT_WIDTH/2 - fontSize/2;
      int centerRectY = rectCoordY + GUESS_RECT_HEIGHT/2 - fontSize/2;
      char currentCharacter = keysClicked[keyClickedNdx];
      DrawTextCodepoint(Raylib::GetFontDefault(), currentCharacter, {(float)centerRectX, (float)centerRectY}, fontSize, Raylib::WHITE);
    }

    //draw OK button and handle game logic
    {
      int rectCoordX = KEYBOARD_WIDTH * KEYBOARD_RECT_WIDTH + (WINDOW_WIDTH - KEYBOARD_RECT_WIDTH * KEYBOARD_WIDTH)/2;
      int rectCoordY = WINDOW_HEIGHT - KEYBOARD_HEIGHT * KEYBOARD_RECT_HEIGHT + /*floor*/(KEYBOARD_HEIGHT / 2) * KEYBOARD_RECT_HEIGHT ;
      int rectWidth = KEYBOARD_RECT_WIDTH - KEYBOARD_RECT_PADDING_WIDTH;
      int rectHeight = KEYBOARD_RECT_HEIGHT - KEYBOARD_RECT_PADDING_HEIGHT;
      Raylib::DrawRectangle(rectCoordX, rectCoordY, rectWidth, rectHeight, Raylib::BLACK);

      int fontSize = 18;
      int centerRectX = rectCoordX + KEYBOARD_RECT_WIDTH/2 - fontSize/2;
      int centerRectY = rectCoordY + KEYBOARD_RECT_HEIGHT/2 - fontSize/2;
      Raylib::DrawText("OK", centerRectX, centerRectY, fontSize, Raylib::WHITE);

      int mouseX = Raylib::GetMouseX();
      int mouseY = Raylib::GetMouseY();
      bool isWithinRegion = (mouseX > rectCoordX) && (mouseX < (rectCoordX + rectWidth)) && (mouseY > rectCoordY) && (mouseY < (rectCoordY + rectHeight));

      if(isWithinRegion && IsMouseButtonReleased(Raylib::MOUSE_BUTTON_LEFT) && isFinishedRow){
	const char *playerGuess = keysClicked + numGuesses*WORD_LENGTH;

	if(Wordle::verify_input(playerGuess, words)){

	  Wordle::Color colors[] = {Wordle::W_GRAY, Wordle::W_GRAY, Wordle::W_GRAY, Wordle::W_GRAY, Wordle::W_GRAY};
	  Wordle::game_logic(playerGuess, chosen, colors);

	  for(int colorIndex=0; colorIndex<WORD_LENGTH; colorIndex++)
	    guessRectColors[numGuesses*WORD_LENGTH + colorIndex] = wordleColorToRaylibColor[colors[colorIndex]];
	  
	  numGuesses++;

	  Wordle::GameState state = Wordle::game_state(colors, numGuesses);
	  msgIndex = gameStateToMsg[state];
	}
	else msgIndex = INVALID_INPUT_MSG;
      }
    }

    Raylib::DrawText(msgs[msgIndex], 0, 0, 24, Raylib::WHITE);

    Raylib::EndDrawing();
  }

  Raylib::CloseWindow();

  return 0;
}
