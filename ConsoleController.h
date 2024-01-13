#pragma once
#include <Windows.h>

#define DEFAULT_SCREEN_WIDTH (120)
#define DEFAULT_SCREEN_HEIGHT (40)

class ConsoleController
{
public:
	static const int nScreenWidth;
	static const int nScreenHeight;
	static WCHAR* screen;
	static HANDLE hConsole;
	static DWORD dwBytesWritten;

	ConsoleController();
	ConsoleController(int width, int height);
	ConsoleController(ConsoleController& exemp);
	~ConsoleController();

	static void UpdateFrame();
	
private:
};
