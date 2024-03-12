#include <iostream>
#include <string>

#include "FastConsole/FastConsole.h"
using namespace fastconsole;

#include "ANSI/ANSIConsole.h"
using namespace ansiconsole;

int main()
{
	FastConsole print;

	print(L'\u2588', 0, 0, BLUE);
	print(L'\u2591');
	print(L'\u2592', 0, 1, LIGHTMAGENTA);
	print(L'\u2593');

	print("hello world", 0, 2);
	getchar();
	
	print("H", 0, 2);
	print("W", 6, 2);
	print("!", 11, 2);
	getchar();
	
	print.Clear();
	

	ANSIConsole draw;
	draw.Print("ANSI output");
	getchar();

	for (int x = 0; x < 3; ++x)
	{
		int color = 0;
		int bgColor = 0;
		for (int i = 0; i < 1000; ++i)
		{
			draw.SetConsoleAtributes(ConsoleFlags::Reset);
			draw.SetConsoleTextColor(color, bgColor);
			draw.Print("Hello ");


			draw.SetConsoleAtributes(ConsoleFlags::Inverse | ConsoleFlags::Bold | ConsoleFlags::Reset);
			draw.SetConsoleTextColor(color, bgColor);
			draw.Print(L"\u2588");
			draw.Print(L"\u2591");
			draw.Print(L"\u2592");
			draw.Print(L"\u2593");

			++bgColor;
			if (bgColor == 10)
			{
				bgColor = 0;
				++color;

				if (color == 10)
					color = 0;
			}

		}

		draw.SetConsoleTextNormal();
	}
	getchar();
	draw.Clear();
	draw.SetConsoleTextNormal();
	draw.Print("Job done.");

	draw.SetConsoleCursorPos({ 0, 2 });
	draw.SetConsoleAtributes(ConsoleFlags::Italic | ConsoleFlags::Underline);
	draw.SetConsoleTextColor(ANSIConsole::Colors::Black, ANSIConsole::Colors::White);
	draw.Print("press enter");
	draw.SetConsoleTextNormal();

	return 0;
}