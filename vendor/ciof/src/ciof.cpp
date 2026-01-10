#include <iostream>

#include "../inc/ciof/ciof.hpp"

#ifndef __CIOF_OK
# 	error "CIOF Is not OK, make sure to not have any errors at compile time"
#endif // NOT : __CIOF_OK

#ifdef CIOF_OS_WIN32
# 	include <windows.h>
# 	include <VersionHelpers.h>
#endif

namespace ciof
{
	CIOFOutputConfig outputConf;

	void print()
	{ std::cout << std::endl; }

	std::string getCursorPos(int _row, int _col)
	{ return std::string("\033[" + std::to_string(_row) + ";" + std::to_string(_col) + "H"); }

	void cursorPos(int _row, int _col)
	{ std::cout << getCursorPos(_row, _col) << std::flush; }

	void initANSI()
	{
# 	ifdef CIOF_OS_WIN32
		// NOTE:
		// This must ONLY be used in Windows 10 or newer
		if (!IsWindows10OrGreater()) return;
		
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;

		GetConsoleMode(hOut, &dwMode);

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

		SetConsoleMode(hOut, dwMode);
# 	endif
	}
	
	std::string styleSet(const OutputStyle &_style)
	{
		switch (_style)
		{
			case ciof::OutputStyle::Bold:  					return "\033[1m"; 
			case ciof::OutputStyle::Italic:  				return "\033[3m"; 
			case ciof::OutputStyle::Underline:  			return "\033[4m"; 
			case ciof::OutputStyle::Strikethrough:  		return "\033[9m"; 
			default:  										return "";
		}
	}

	std::string styleReset()
	{ return "\033[0m"; }

	std::string colorSet(int _color)
	{ return "\033[" + std::to_string(_color) + "m"; }

	std::string rgbSet(unsigned int r, unsigned int g, unsigned int b)
	{
		return "\033[38;2;" + std::to_string(r) + ";"
			+ std::to_string(g) + ";"
			+ std::to_string(b) + "m";
	}
}
