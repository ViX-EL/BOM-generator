#include "ConsolPrinter.h"

#include <iostream>
#include <string>

ConsolPrinter::ConsolPrinter()
{
	std::wcout.imbue(std::locale("rus_rus.866"));
	std::wcin.imbue(std::locale("rus_rus.866"));
}

void ConsolPrinter::printError(const std::wstring& message) const
{
	std::wcout << L"Îøèáêà: " << message << '\n';
}

void ConsolPrinter::printText(const std::wstring& text, const std::wstring& label)
{
	std::wcout << label << "\n\n" << text << '\n';
}


