#pragma once

#include <wx/wx.h>

#include <string>

class TextDialog : public wxFrame
{
public:

	TextDialog(const wxString& title);

	const wxString& GetText();
	void setText(const std::wstring& text);
	void OnCopyClick(wxCommandEvent& event);

private:

	wxTextCtrl* textBox;
};

