#include "WXLogPrinter.h"
#include <wx/wx.h>

void WXLogPrinter::printError(const std::wstring& message) const
{
	wxLogError("%s\n", message);
}

void WXLogPrinter::printText(const std::wstring& message, const std::wstring& label)
{
	wxLogMessage("%s %s\n", label, message);
}
