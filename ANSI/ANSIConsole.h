#pragma once

#include <Windows.h>
#include <string>

namespace ansiconsole
{

	//ANSI escape codes: https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
#define WIDECHAR

	enum class ConsoleFlags : std::uint16_t
	{
		None = 1 << 0
		, Reset = 1 << 1
		, Bold = 1 << 2
		, Dim = 1 << 3
		, Italic = 1 << 4
		, Underline = 1 << 5
		, Blinking = 1 << 6
		, Inverse = 1 << 7
		, Hidden = 1 << 8
		, StrikeThrough = 1 << 9
	};
	inline ConsoleFlags operator |(ConsoleFlags lhs, ConsoleFlags rhs)
	{
		using falg = std::underlying_type<ConsoleFlags>::type;
		return static_cast<ConsoleFlags>(static_cast<falg>(lhs) | static_cast<falg>(rhs));
	}

	inline bool operator &(ConsoleFlags lhs, ConsoleFlags rhs)
	{
		using flag = std::underlying_type<ConsoleFlags>::type;
		return static_cast<bool>(static_cast<flag>(lhs) & static_cast<flag>(rhs));
	}


    // ANSIConsole uses ANSI escape codes to display characters with different colors
    //  This is only supported in some consoles. Use FastConsole for Windows and ANSI for Lunix
	class ANSIConsole
	{
	private:
		HANDLE m_hOutput;
		const std::string m_escPrefix = "\033";

	public:
		enum class Colors
		{
			Black
			, Red
			, Green
			, Yellow
			, Blue
			, Magenta
			, Cyan
			, White
		};


		ANSIConsole();

		void SetConsoleHandle(HANDLE output);

		void SetConsoleCursorPos(COORD coord);

		void SetConsoleTextColor(Colors FGColor, Colors BGColor);
		void SetConsoleTextColor(Colors FGColor);

		//may be slower/ does a to_string call
		void SetConsoleTextColor(unsigned short FGColor, unsigned short BGColor);
		void SetConsoleTextColor(unsigned short FGColor);

		void SetConsoleTextNormal();
		void SetConsoleAtributes(ConsoleFlags flags);

		void Print(std::string disp);//resets text after
#ifdef WIDECHAR
		void Print(std::wstring disp);//for more symbols
#endif
		std::string GetColorCode(Colors color, bool isBackground = false, bool isBright = false);

		void Clear();
	};

} // namespace ansi