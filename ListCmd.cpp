#include "StdAfx.h"
#include "ListCmd.h"
#include "CmdLine.h"

#include "DirIterator.h"
#include <fstream>
#include <iomanip>


namespace ListCmd
{
using JStd::CmdLine::CmdLine;
using namespace std;

void cmd(CmdLine& cmdLine);

static bool registered = JStd::CmdLine::Register(L"list", cmd);

struct Ctxt
{
	size_t headerSz;
	size_t footerSz;
	wstring dir;
	bool headerAsText;
	bool footerAsText;
};

void dumpStr(Ctxt& ctxt, istream& str, wostream& ostr, size_t length, bool asText)
{
	if(asText)
	{
		string text;
		text.resize(length);
		str.read(&*text.begin(),length);
		ostr << JStd::String::ToWide(text, CP_ACP);
	}
	else for(size_t i=0; i<length && str; ++i)
	{
		char b = 0;
		str.read(&b, 1);
		ostr << setw(2) << setfill(L'0') << hex << (int)(unsigned char)b << " ";
	}
}

void dumpHeaderFooter(Ctxt& ctxt, const wstring& file)
{
	wstring::size_type slash = ctxt.dir.find_last_of(L'\\');
	wstring path;
	if(slash == wstring::npos)
		path = file;
	else
		path = ctxt.dir.substr(0,slash + 1) + file;

	ifstream fileStr(JStd::String::ToMult(path, CP_UTF8).c_str(), ios::binary | ios::in);
	dumpStr(ctxt, fileStr, wcout, ctxt.headerSz, ctxt.headerAsText);

	wcout << "- ";
	fileStr.clear();
	fileStr.seekg(0, ios::end);
	fileStr.seekg(fileStr.tellg() - (std::streamoff)ctxt.footerSz);
	dumpStr(ctxt, fileStr, wcout, ctxt.footerSz, ctxt.footerAsText);
}

void cmd(CmdLine& cmdLine)
{
	wchar_t* dummy;

	Ctxt ctxt;
	ctxt.headerSz		= wcstol(cmdLine.next(L'h', L"hsize").c_str(), &dummy, 10);
	ctxt.footerSz		= wcstol(cmdLine.next(L'f', L"fsize").c_str(), &dummy, 10);
	ctxt.dir			= cmdLine.next();
	ctxt.headerAsText	= cmdLine.hasOption(L"headerText");
	ctxt.footerAsText	= cmdLine.hasOption(L"footerText");
	if(cmdLine.hasOption(L't', L"text"))
		ctxt.headerAsText = ctxt.footerAsText = true;

	if(!ctxt.headerSz) ctxt.headerSz = 10;
	if(!ctxt.footerSz) ctxt.footerSz = 10;

	if(ctxt.dir.empty())
		JStd::CmdLine::throwUsage(L"HeaderFooter list <file> [-h <header size>] [-f <footer size>]");

	for(JStd::CDirIterator i(ctxt.dir.c_str()); i; ++i)
	{
		if(i.IsDirectory()) continue;
		//wcout << i.File().name << L": ";
		dumpHeaderFooter(ctxt, i.File().name);
		wcout << "- " << i.File().name;
		wcout << endl;
	}
}

}