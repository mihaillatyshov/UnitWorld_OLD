#include "ConsoleLog.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace LM
{
	void ConsoleLog::SetColor(ConsoleColorType txt, ConsoleColorType bg)
	{
#ifdef _WIN32
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdOut, (WORD)(((int)bg << 4) | ((int)txt)));
#else
		std::string style = "\033[" + txt + ";" + bg + "m";
		std::cerr << style;
		//std::cerr << "\033[0m \n"; // To reset atr
#endif
	}

}