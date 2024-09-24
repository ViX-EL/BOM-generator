#include "TextDialog.h"

#include <wx/clipbrd.h>

TextDialog::TextDialog(const wxString& title) : wxFrame(NULL, -1, title, wxDefaultPosition, wxSize(600, 700))
{
	wxPanel* panelMain = new wxPanel(this, -1);
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	panelMain->SetSizer(vbox);

	wxPanel* panelButtons = new wxPanel(panelMain, -1);
	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	panelButtons->SetSizer(hbox);

	wxButton* copyBtn = new wxButton(panelButtons, -1, wxT("Копировать текст"));
	hbox->Add(copyBtn, 1, wxALL | wxALIGN_LEFT, 5);

	textBox = new wxTextCtrl(panelMain, -1, wxT(""), wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE);

	vbox->Add(panelButtons, 0, wxALL, 5);
	vbox->Add(textBox, 1, wxALL | wxEXPAND | wxCENTER, 5);

	copyBtn->Bind(wxEVT_BUTTON, &TextDialog::OnCopyClick, this);
	Center();
}

const wxString& TextDialog::GetText()
{
	return textBox->GetLabelText();
}

void TextDialog::setText(const std::wstring& text)
{
	textBox->SetLabelText(text);
}

void TextDialog::OnCopyClick(wxCommandEvent& event)
{
	if (wxTheClipboard->Open()) 
	{
		wxTheClipboard->SetData(new wxTextDataObject(textBox->GetLabelText()));
		wxTheClipboard->Close();
	}
}
