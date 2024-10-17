#include "DialogPrinter.h"

#include "TextDialog.h"
#include "StringConvert.h"

#include <wx/wx.h>
#include <string>

DialogPrinter::DialogPrinter()
{
	textDialog = new TextDialog(L"");
}

void DialogPrinter::printError(const std::wstring& message) const
{
	wxMessageBox(message, "Îøèáêà", wxOK | wxICON_ERROR);
}

void DialogPrinter::printText(const std::wstring& text, const std::wstring& label)
{
	textDialog->SetLabel(label);
	textDialog->setText(text);
	if (!textDialog->IsVisible()) {
		textDialog->Show(true);
	}
}
