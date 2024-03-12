#include "FastConsole.h"

#include <iostream>
#include <io.h>//setmode
#include <fcntl.h>//_O_ codes

namespace fastconsole
{

	FastConsole::FastConsole()
		:outputHandle(GetStdHandle(STD_OUTPUT_HANDLE))
		, coord({ 0,0 })
	{
		UpdatePos();
	}

	void FastConsole::UpdatePos()
	{
		POINT pos;
		if (GetCursorPos(&pos))
		{
			coord.X = static_cast<short>(pos.x);
			coord.Y = static_cast<short>(pos.y);
		}
	}

	////////////////////////////////////////////////////////////
	//basic print
	//////////////////////////////////////////////////////////

	////////////////////////////////////
	//print char at location on screen
	//////////////////////////////////
	void FastConsole::operator()(char disp, int x, int y)
	{
		coord = { (short)x, (short)y };
		SetConsoleCursorPosition(outputHandle, coord);
		char buff[1]{ disp };

		//draw buff to consol
		WriteConsoleA(outputHandle, buff, 1, NULL, NULL);
	}

	//loop through and display string starting at location
	void FastConsole::operator()(std::string disp, int x, int y)
	{
		for (unsigned int i = 0; i < disp.length(); ++i)
		{
			(*this)(disp[i], x + i, y);
		}
	}

	//make int into string then print
	void FastConsole::operator()(int disp, int x, int y)
	{
		std::string val = std::to_string(disp);
		(*this)(val, x, y);
	}


	void FastConsole::operator()(char disp)
	{
		UpdatePos();
		(*this)(disp, coord.X, coord.Y);
	}

	void FastConsole::operator()(std::string disp)
	{
		UpdatePos();
		(*this)(disp, coord.X, coord.Y);
	}

	void FastConsole::operator()(int disp)
	{
		UpdatePos();
		(*this)(disp, coord.X, coord.Y);
	}


#ifdef COLOR
	////////////////////////////////////////////////////////////
	//color print
	//////////////////////////////////////////////////////////

	void FastConsole::operator()(char disp, int x, int y, int color)
	{
		SetConsoleTextAttribute(outputHandle, color);//set color
		(*this)(disp, x, y);
		SetConsoleTextAttribute(outputHandle, LIGHTGREY);//set back to default color
	}

	void FastConsole::operator()(int disp, int x, int y, int color)
	{
		SetConsoleTextAttribute(outputHandle, color);
		(*this)(disp, x, y);
		SetConsoleTextAttribute(outputHandle, LIGHTGREY);
	}

	void FastConsole::operator()(std::string disp, int x, int y, int color)
	{
		SetConsoleTextAttribute(outputHandle, color);
		(*this)(disp, x, y);
		SetConsoleTextAttribute(outputHandle, LIGHTGREY);
	}


	void FastConsole::operator()(char disp, int color)
	{
		SetConsoleTextAttribute(outputHandle, color);
		(*this)(disp);
		SetConsoleTextAttribute(outputHandle, LIGHTGREY);
	}

	void FastConsole::operator()(int disp, int color)
	{
		SetConsoleTextAttribute(outputHandle, color);
		(*this)(disp);
		SetConsoleTextAttribute(outputHandle, LIGHTGREY);
	}

	void FastConsole::operator()(std::string disp, int color)
	{
		SetConsoleTextAttribute(outputHandle, color);
		(*this)(disp);
		SetConsoleTextAttribute(outputHandle, LIGHTGREY);
	}
#endif

#ifdef WIDECHAR
	////////////////////////////////////////////////////////////
	//wide print
	//////////////////////////////////////////////////////////

	void FastConsole::operator()(wchar_t disp, int x, int y)
	{
		_setmode(_fileno(stdout), _O_WTEXT);

		coord = { (short)x, (short)y };
		SetConsoleCursorPosition(outputHandle, coord);
		wchar_t buff[1]{ disp };

		WriteConsoleW(outputHandle, buff, 1, NULL, NULL);

		_setmode(_fileno(stdout), _O_TEXT);
	}

	void FastConsole::operator()(wchar_t disp)
	{
		UpdatePos();
		(*this)(disp, coord.X, coord.Y);
	}


#ifdef COLOR
	//print wide char with color
	void FastConsole::operator()(wchar_t disp, int x, int y, int color)
	{
		SetConsoleTextAttribute(outputHandle, color);
		(*this)(disp, x, y);
		SetConsoleTextAttribute(outputHandle, LIGHTGREY);
	}
#endif // COLOR
#endif // WIDECHAR

	void FastConsole::Clear() {
		COORD topLeft = { 0, 0 };
		CONSOLE_SCREEN_BUFFER_INFO screen;
		DWORD written;

		GetConsoleScreenBufferInfo(outputHandle, &screen);
		FillConsoleOutputCharacterA(outputHandle, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
		FillConsoleOutputAttribute(outputHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
		SetConsoleCursorPosition(outputHandle, topLeft);
	}

} // namespace fastconsole