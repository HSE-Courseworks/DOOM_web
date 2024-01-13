#include "ConsoleController.h"


const int ConsoleController::nScreenHeight = DEFAULT_SCREEN_HEIGHT;
const int ConsoleController::nScreenWidth = DEFAULT_SCREEN_WIDTH;
WCHAR* ConsoleController::screen;
HANDLE ConsoleController::hConsole;
DWORD ConsoleController::dwBytesWritten = 0;

ConsoleController::ConsoleController() /*: nScreenWidth(DEFAULT_SCREEN_WIDTH), nScreenHeight(DEFAULT_SCREEN_HEIGHT)*/
{
	screen = new WCHAR[nScreenWidth * nScreenHeight + 1]; // Cимволы юникода
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	dwBytesWritten = 0;
	SetConsoleScreenBufferSize(hConsole, { DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT });
}

ConsoleController::ConsoleController(int width, int height) /*: nScreenWidth(width), nScreenHeight(height)*/
{
	screen = new WCHAR[nScreenWidth * nScreenHeight + 1]; // Cимволы юникода
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	dwBytesWritten = 0;
	SetConsoleScreenBufferSize(hConsole, { (short)width, (short)height });
}

ConsoleController::ConsoleController(ConsoleController& exemp)
{
	screen = exemp.screen;
	hConsole = exemp.hConsole;
	dwBytesWritten = exemp.dwBytesWritten;
}

ConsoleController::~ConsoleController()
{
	delete[] screen;
}

void ConsoleController::UpdateFrame()
{
	screen[nScreenWidth * nScreenHeight] = '\0';  // Последний символ - окончание строки
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
}
