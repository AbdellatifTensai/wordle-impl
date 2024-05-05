CFLAGS = -O2 -Wall -Wextra -pedantic
DEBUG_CFLAGS = -Wall -Wextra -pedantic -g

wordle-terminal:
	g++ $(CFLAGS) src/wordle.h src/wordle.cpp src/wordle-terminal.cpp -o wordle-terminal

wordle-raylib:
	g++ $(CFLAGS) `pkg-config raylib --cflags` src/wordle.h src/wordle.cpp src/wordle-raylib.cpp -o wordle-raylib `pkg-config raylib --libs`

wordle-terminal-debug:
	g++ $(DEBUG_CFLAGS) src/wordle.h src/wordle.cpp src/wordle-terminal.cpp -o wordle-terminal-debug

wordle-raylib-debug:
	g++ $(DEBUG_CFLAGS) `pkg-config raylib --cflags` src/wordle.h src/wordle.cpp src/wordle-raylib.cpp -o wordle-raylib-debug `pkg-config raylib --libs`
