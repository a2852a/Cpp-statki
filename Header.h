#pragma once
#include <windows.h>
#include <objidl.h>

#define errhandler()

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void ClearScreen() // Cala fukncja do skopiowania - Windows.h!!!
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

void SetConsoleCoords(COORD xy)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}

COORD GetCurCoords(void)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	return csbi.dwCursorPosition;

}

	void move_cursor_offset_X(int oX = 0) { // Przenies kursor o X pozycji poziomo
		COORD bufcords;
		bufcords = GetCurCoords();
		bufcords.X += oX;
		SetConsoleCoords(bufcords);

	}

	void move_cursor_offset_Y(int oY = 0) { // Przenies kursor o X pozycji poziomo
		COORD bufcords;
		bufcords = GetCurCoords();
		bufcords.Y += oY;
		SetConsoleCoords(bufcords);

	}

	void move_cursor_offset_XY(int oX, int oY) {
		move_cursor_offset_X(oX);
		move_cursor_offset_Y(oY);

	}
