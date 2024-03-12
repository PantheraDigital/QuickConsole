#include "ANSIConsole.h"
#include <iostream>

namespace ansiconsole
{

	std::string ANSIConsole::GetColorCode(Colors color, bool isBackground, bool isBright)
	{
		std::string colorCode = "";

		if (isBackground)
		{
			if (isBright)
				colorCode += "10";
			else
				colorCode += "4";
		}
		else
		{
			if (isBright)
				colorCode += "9";
			else
				colorCode += "3";
		}

		switch (color)
		{
		case ANSIConsole::Colors::Black:
			colorCode += "0";
			break;
		case ANSIConsole::Colors::Red:
			colorCode += "1";
			break;
		case ANSIConsole::Colors::Green:
			colorCode += "2";
			break;
		case ANSIConsole::Colors::Yellow:
			colorCode += "3";
			break;
		case ANSIConsole::Colors::Blue:
			colorCode += "4";
			break;
		case ANSIConsole::Colors::Magenta:
			colorCode += "5";
			break;
		case ANSIConsole::Colors::Cyan:
			colorCode += "6";
			break;
		case ANSIConsole::Colors::White:
			colorCode += "7";
			break;
		default:
			colorCode += "7";
			break;
		}

		return colorCode;
	}


	ANSIConsole::ANSIConsole()
		:m_hOutput(GetStdHandle(STD_OUTPUT_HANDLE))
	{
	}

	void ANSIConsole::SetConsoleHandle(HANDLE output)
	{
		m_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void ANSIConsole::SetConsoleCursorPos(COORD coord)
	{
		SetConsoleCursorPosition(m_hOutput, coord);
	}

	void ANSIConsole::SetConsoleTextColor(Colors FGColor, Colors BGColor)
	{
		Print(m_escPrefix + "[2;" + GetColorCode(FGColor) + ";" + GetColorCode(BGColor, true) + "m");
	}
	void ANSIConsole::SetConsoleTextColor(Colors FGColor)
	{
		Print(m_escPrefix + "[" + GetColorCode(FGColor) + "m");
	}

	void ANSIConsole::SetConsoleTextColor(unsigned short FGColor, unsigned short BGColor)
	{
		if ((FGColor <= 9 && FGColor >= 0) && (BGColor <= 9 && BGColor >= 0))
			Print(m_escPrefix + "[2;3" + std::to_string(FGColor) + ";4" + std::to_string(BGColor) + "m");
	}
	void ANSIConsole::SetConsoleTextColor(unsigned short FGColor)
	{
		if (FGColor <= 9 && FGColor >= 0)
			Print(m_escPrefix + "[3" + std::to_string(FGColor) + "m");
	}

	void ANSIConsole::SetConsoleAtributes(ConsoleFlags flags)
	{
		std::string codes = "";
		if (flags & ConsoleFlags::Reset)
			SetConsoleTextNormal();

		if (flags & ConsoleFlags::Bold)
			codes.append("1;");

		if (flags & ConsoleFlags::Dim)
			codes.append("2;");

		if (flags & ConsoleFlags::Italic)
			codes.append("3;");

		if (flags & ConsoleFlags::Underline)
			codes.append("4;");

		if (flags & ConsoleFlags::Blinking)
			codes.append("5;");

		if (flags & ConsoleFlags::Inverse)
			codes.append("7;");

		if (flags & ConsoleFlags::Hidden)
			codes.append("8;");

		if (flags & ConsoleFlags::StrikeThrough)
			codes.append("9");

		if (codes.length() > 0 && codes.at(codes.length() - 1) == ';')
			codes.pop_back();

		Print(m_escPrefix + "[" + codes + "m");
	}

	void ANSIConsole::SetConsoleTextNormal()
	{
		Print(m_escPrefix + "[0m");
	}

	void ANSIConsole::Print(std::string disp)
	{
		WriteConsoleA(m_hOutput, disp.c_str(), disp.length(), NULL, NULL);
	}

	void ANSIConsole::Print(std::wstring disp)
	{
		WriteConsoleW(m_hOutput, disp.c_str(), 1, NULL, NULL);
	}

	void ANSIConsole::Clear()
	{
		//code from: https://stackoverflow.com/questions/34842526/update-console-without-flickering-c
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD topLeft = { 0, 0 };

		// std::cout uses a buffer to batch writes to the underlying console.
		// We need to flush that to the console because we're circumventing
		// std::cout entirely; after we clear the console, we don't want
		// stale buffered text to randomly be written out.
		std::cout.flush();

		// Figure out the current width and height of the console window
		if (!GetConsoleScreenBufferInfo(m_hOutput, &csbi)) {
			// TODO: Handle failure!
			abort();
		}
		DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

		DWORD written;

		// Flood-fill the console with spaces to clear it
		FillConsoleOutputCharacter(m_hOutput, TEXT(' '), length, topLeft, &written);

		// Reset the attributes of every character to the default.
		// This clears all background colour formatting, if any.
		FillConsoleOutputAttribute(m_hOutput, csbi.wAttributes, length, topLeft, &written);

		// Move the cursor back to the top left for the next sequence of writes
		SetConsoleCursorPosition(m_hOutput, topLeft);
	}

} // namespace ansi