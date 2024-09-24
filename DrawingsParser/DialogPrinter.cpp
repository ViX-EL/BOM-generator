#include "DialogPrinter.h"

#include "TextDialog.h"
#include "StringConvert.h"

#include <wx/wx.h>
#include <string>

void DialogPrinter::printError(const std::wstring& message) const
{
	wxMessageBox(message, "Ошибка", wxOK | wxICON_ERROR);
}

void DialogPrinter::printText(const std::wstring& text, const std::wstring& label)
{
	//wxMessageBox(message, "Информация", wxOK | wxICON_INFORMATION);
	TextDialog* textDialog = new TextDialog(utf8_encode(label));
	textDialog->setText(text);
	textDialog->Show(true);
}
