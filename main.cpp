#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>

#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30

#define DEAD '.'
#define ALIVE '#'

typedef struct consolePoint {
	int x;
	int y;
};

void set(wchar_t* screen, char character, consolePoint coords) {
	screen[SCREEN_WIDTH * coords.y + coords.x] = character;
}

void BoardToScreen(char board[SCREEN_WIDTH][SCREEN_HEIGHT], wchar_t* screen) {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			screen[SCREEN_WIDTH * y + x] = (wchar_t)board[x][y];
		}
	}
}

void changeLiveStatusPoint(char board[SCREEN_WIDTH][SCREEN_HEIGHT], consolePoint coords) {
	int neighboursAlive = 0;
	
	if (coords.x > 0 && board[coords.x - 1][coords.y] == ALIVE) {
		neighboursAlive++;
	}
	if (coords.x < SCREEN_WIDTH && board[coords.x + 1][coords.y] == ALIVE) {
		neighboursAlive++;
	}

	if (coords.y > 0 && board[coords.x][coords.y - 1] == ALIVE) {
		neighboursAlive++;
	}
	if (coords.y < SCREEN_HEIGHT && board[coords.y][coords.y + 1] == ALIVE) {
		neighboursAlive++;
	}

	if ((coords.x > 0 && coords.y > 0) && board[coords.x - 1][coords.y - 1] == ALIVE) {
		neighboursAlive++;
	}
	if ((coords.x < SCREEN_WIDTH && coords.y < SCREEN_HEIGHT) && board[coords.x + 1][coords.y + 1] == ALIVE) {
		neighboursAlive++;
	}

	if ((coords.x < SCREEN_WIDTH && coords.y > 0) && board[coords.x + 1][coords.y - 1] == ALIVE) {
		neighboursAlive++;
	}
	if ((coords.x > 0 && coords.y < SCREEN_HEIGHT) && board[coords.x - 1][coords.y + 1] == ALIVE) {
		neighboursAlive++;
	}

	if (neighboursAlive == 3) {
		board[coords.x][coords.y] = ALIVE;
	}
	else if (neighboursAlive > 3 || neighboursAlive < 2) {
		board[coords.x][coords.y] = DEAD;
	}
}

int main() {
	// Making the board.
	char board[SCREEN_WIDTH][SCREEN_HEIGHT];

	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			board[x][y] = (rand() % 2) == 1? ALIVE: DEAD;
		}
	}

	// Making a console screenbuffer.
	wchar_t* screen = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT]{};
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Main loop.
	while (_getch() != 'x') {
		// Checking if they should live.
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			for (int y = 0; y < SCREEN_HEIGHT; y++) {
				changeLiveStatusPoint(board, {x, y});
			}
		}

		// Write to the screen.
		BoardToScreen(board, screen);
		WriteConsoleOutputCharacter(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0, 0 }, &dwBytesWritten);
	}

	delete[] screen;
	return 0;
}