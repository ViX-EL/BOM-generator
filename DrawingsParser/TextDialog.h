#pragma once

#ifndef TextDialog_h__
#define TextDialog_h__

#include <wx/wx.h>

#include <string>

class TextDialog : public wxFrame
{
public:

	explicit TextDialog(const wxString& title);

	const wxString& GetText();
	void setText(const std::wstring& text);
	void addText(const std::wstring& text);
	void OnCopyClick(wxCommandEvent& event);

private:

	wxTextCtrl* textBox;
};

#endif // TextDialog_h__

