#pragma once

#include <Windows.h>
#include <string>

namespace fastconsole
{
	#define WIDECHAR
	#define COLOR

	#ifdef COLOR
		#define BLACK 0
		#define BLUE 1
		#define GREEN 2
		#define CYAN 3
		#define RED 4
		#define MAGENTA 5
		#define BROWN 6

		#define LIGHTGREY 7 //default
		#define DARKGREY 8

		#define LIGHTBLUE 9
		#define LIGHTGREEN 10
		#define LIGHTCYAN 11
		#define LIGHTRED 12
		#define LIGHTMAGENTA 13
		#define YELLOW 14

		#define WHITE 15
	#endif

	
	class FastConsole
	{
		HANDLE outputHandle;
		COORD coord;

		//sets coord to cursor pos
		void UpdatePos();

	public:

		FastConsole();

		////////////////////////////////////////////////////////////
		//basic print
		//////////////////////////////////////////////////////////
		
		////////////////////////////////////
		//print at location on screen
		//////////////////////////////////
		void operator()(char disp, int x, int y);
		void operator()(std::string disp, int x, int y);
		void operator()(int disp, int x, int y);
		///////////////////////////////
		//print at last cursor location
		////////////////////////////////
		void operator()(char disp);
		void operator()(std::string disp);
		void operator()(int disp);


		////////////////////////////////////////////////////////////
		//color print
		//////////////////////////////////////////////////////////
	#ifdef COLOR
		void operator()(char disp, int x, int y, int color);
		void operator()(char disp, int color);

		void operator()(int disp, int x, int y, int color);
		void operator()(int disp, int color);

		void operator()(std::string disp, int x, int y, int color);
		void operator()(std::string disp, int color);

	#endif // COLOR

		////////////////////////////////////////////////////////////
		//wide print
		//////////////////////////////////////////////////////////
	#ifdef WIDECHAR
		//print wide char at location
		void operator()(wchar_t disp, int x, int y);

		//print at last cursor location
		void operator()(wchar_t disp);

	#ifdef COLOR
		//wide print with color
		void operator()(wchar_t disp, int x, int y, int color);
	#endif // COLOR

	#endif // WIDECHAR

		void Clear();
	};

} // namespace fastconsole