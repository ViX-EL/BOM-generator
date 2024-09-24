#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxSize& size);

	void OnSelectInputFolder(wxCommandEvent& event);
	void OnSelectOutputFolder(wxCommandEvent& event);
	void OnCreateXlsxTable(wxCommandEvent& event);

private:
	void OnAbout(wxCommandEvent& event);

	void selectPath(wxTextCtrl* textField);
	bool isPathCorrect(wxTextCtrl* textField, const wxString& errMessage);
	void getFileNames();

	wxTextCtrl* inputFolderTextField;
	wxTextCtrl* outputFolderTextField;
	std::vector<wxString> filesNames;
	wxString aboutStr{ "\n��������� ������ ��������� dwg ������ �������������� �������� "
		"������������� �� ������� ����� � ����������� ����������� ������ � "
		"���� xlsx (���� Excel) ������� � �������� �����.\n\n��� �������� Excel ������� ������� "
		"���������� ������������� �������� ������� � �������� "
		"����� � ������� ������ \"������� Excel �������\". �������� ������ �������� ��� ������"
		"�� ��������������� \"LGN\", \"ASP\", \"IOT\", \"NAG\", \"PTE\". \n\n�� ����� ��������� ������ ����� "
		"����������� ������. ��������� �� ������� �� ����� - Dmitry22_s@cc7.cn ��� Teams - Dmitriy Zelenev, � ������ ���������� ��� "
		"������ - ���� �� �������������� / ������ � �������� / ������, ����������� ������������ "
		"���������� ��������, ���� ��� ����, � ��� �� �������� ��� ����� � ��������� ������ ������ � �������."};
};

