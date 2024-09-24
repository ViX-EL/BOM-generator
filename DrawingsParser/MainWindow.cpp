#include "MainWindow.h"

#include "DWGTextLoader.h"
#include "TextParser.h"
#include "TableWriter.h"
#include "DialogPrinter.h"

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/aboutdlg.h> 
#include <regex>


MainWindow::MainWindow(const wxString& title, const wxSize& size) :
	wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size, 
		wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	Centre();

	wxMenu* menuAbout = new wxMenu;
	menuAbout->Append(wxID_ABOUT, "О программе");

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuAbout, "Справка");

	SetMenuBar(menuBar);

	Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);

	wxStaticText* inputFolderLabel = new wxStaticText(this, wxID_ANY, "Выберите папку с dwg файлами",
		wxPoint(10, 5));

	inputFolderTextField = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10,25), wxSize(300, 20));
	inputFolderTextField->AutoCompleteDirectories();

	wxButton* buttonInputFolder = new wxButton(this, 1, wxT("Выбор папки"), wxPoint(320, 23));
	buttonInputFolder->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnSelectInputFolder, this);

	wxStaticText* outputFolderLabel = new wxStaticText(this, wxID_ANY,
		"Выберите папку, куда сохранить таблицу", wxPoint(10, 50));

	outputFolderTextField = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 20));

	wxButton* buttonOutputFolder = new wxButton(this, 2, wxT("Выбор папки"), wxPoint(320, 68));
	buttonOutputFolder->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnSelectOutputFolder, this);

	wxButton* buttonCreateTable = new wxButton(this, 3, wxT("Создать Excel таблицу"),
		wxPoint(150, 102));
	buttonCreateTable->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnCreateXlsxTable, this);

	CreateStatusBar();
}

void MainWindow::OnSelectInputFolder(wxCommandEvent& event)
{
	selectPath(inputFolderTextField);
}

void MainWindow::OnSelectOutputFolder(wxCommandEvent& event)
{
	selectPath(outputFolderTextField);
}

void MainWindow::OnCreateXlsxTable(wxCommandEvent& event)
{
	getFileNames();

	if (filesNames.empty()) {
		return;
	}

	DWGTextLoader textLoader(new DialogPrinter);
	TextParser parser(textLoader.getText(), textLoader.getSeparator(), new DialogPrinter);
	for (auto& name : filesNames)
	{
		textLoader.loadFile((inputFolderTextField->GetLabelText() + "\\" + name).ToStdString());
		if (textLoader.getText().empty()) {
			return;
		}
		parser.parse(textLoader.getFileName());
	}

	TableWriter tableWriter(parser.getColumns(), parser.getComponentsCountPerList());
	tableWriter.createNewTableFile(outputFolderTextField->GetLabelText().ToStdString());
	tableWriter.writeTable();

	SetStatusText("Таблица " + tableWriter.getFileName() + " создана!");
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.SetName(_("Bom generator"));
	info.SetVersion(_("0.4.0 Beta"));
	info.SetDescription(_(aboutStr));
	info.SetCopyright("(C) 2024 Dmitriy Zelenev <Dmitry22_s@cc7.cn>");
	wxAboutBox(info);
}

void MainWindow::selectPath(wxTextCtrl* textField)
{
	wxDirDialog dlg(NULL, "Выбрать входной каталог", "/", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal()) {
		textField->SetLabelText(dlg.GetPath());
	}
}

bool MainWindow::isPathCorrect(wxTextCtrl* textField, const wxString& errMessage)
{
	std::wregex textFieldPathPattern(LR"(\w:\\[^/:*?"<>|]+)");
	std::wstring textFolderField = textField->GetLabel().ToStdWstring();
	if (!(textFolderField != "" && std::regex_match(textFolderField, textFieldPathPattern)))
	{
		wxMessageBox("Путь к " + errMessage + " не выбран или указан не верно!",
			"Выберите путь к папке", wxOK | wxICON_EXCLAMATION);
		return false;
	}
	return true;
}

void MainWindow::getFileNames()
{
	filesNames.clear();
	if (!isPathCorrect(inputFolderTextField, "входной папке с dwg файлами")) {
		return;
	}

	wxDir dir(inputFolderTextField->GetLabel());
	wxString fileName;
	bool contains = dir.GetFirst(&fileName, wxEmptyString, wxDIR_FILES);
	if (!contains) {
		wxMessageBox("В папке отсутствуют файлы или путь к папке указан не" 
			" верно!", "Файлы не найдены", wxOK | wxICON_EXCLAMATION);
		return;
	}
	while (contains) {
		if (fileName.ends_with(".dwg")) {
			filesNames.push_back(fileName);
		}
		contains = dir.GetNext(&fileName);
	}

	if (filesNames.size() == 0) {
		wxMessageBox("В папке отсутствуют файлы dwg!", "Файлы не найдены",
			wxOK | wxICON_EXCLAMATION);
	}
}
