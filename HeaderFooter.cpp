// HeaderFooter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include "CmdLine.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	JStd::CmdLine::CallDefaultWithCatch(wcerr, argc, argv);

#ifdef _DEBUG
	_getch();
#endif
	return 0;
}

