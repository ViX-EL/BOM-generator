#include "MainWindow.h"

#include "DWGTextLoader.h"
#include "TextParser.h"
#include "TableWriter.h"
#include "WXLogPrinter.h"

#include <wx/wx.h>
#include <wx/log.h> 
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/aboutdlg.h> 
#include <regex>
#include <filesystem>
#include <unordered_map>
#include <algorithm>
#include <fstream>


MainWindow::MainWindow(const wxString& title, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size, 
		wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	Centre();

	wxLogWindow* logWindow = new wxLogWindow(this, "Log", true, false);
	logWindow->GetFrame()->SetClientSize(650, 400);
	logger.reset(logWindow);
	wxLog::SetActiveTarget(logger.get());
	wxLog::SetLogLevel(wxLOG_Trace);

	wxMenu* menuAbout = new wxMenu;
	menuAbout->Append(wxID_ABOUT, "О программе");

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuAbout, "Справка");

	SetMenuBar(menuBar);

	Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);

	wxStaticText* inputFolderLabel = new wxStaticText(this, wxID_ANY, "Выберите папку с dwg файлами", wxPoint(10, 5));

	inputFolderTextField = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10,25), wxSize(300, 20));
	inputFolderTextField->AutoCompleteDirectories();

	wxButton* buttonInputFolder = new wxButton(this, 1, wxT("Выбор папки"), wxPoint(320, 23));
	buttonInputFolder->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnSelectInputFolder, this);

	wxStaticText* outputFolderLabel = new wxStaticText(this, wxID_ANY, "Выберите папку, куда сохранить таблицу", wxPoint(10, 50));

	outputFolderTextField = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 20));

	wxButton* buttonOutputFolder = new wxButton(this, 2, wxT("Выбор папки"), wxPoint(320, 68));
	buttonOutputFolder->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnSelectOutputFolder, this);

	wxButton* buttonCreateTable = new wxButton(this, 3, wxT("Создать Excel таблицу"), wxPoint(150, 102));
	buttonCreateTable->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainWindow::OnCreateXlsxTable, this);

	CreateStatusBar();

	inputFolderTextField->SetLabelText(R"(C:\Users\User\Downloads\NewIsometricsWithStatusesIFC_AFC\IOT\dwg)");
	outputFolderTextField->SetLabelText(R"(C:\Users\User\Downloads\NewIsometricsWithStatusesIFC_AFC\IOT)");
}

MainWindow::~MainWindow()
{
	wxLog::SetActiveTarget(nullptr);
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

	std::unordered_multimap<wxString, ParsingErrorInfo> filesWithParsingErrors;
	std::vector<wxString> filesWithReadingErrors;
	std::vector<wxString> exceptionsMessages;

	ValuesCheker::setType(ValuesCheker::Type::EXCEPTION);

	DWGTextLoader textLoader(new WXLogPrinter);
	DWGDotNetTextLoader dotNetTextLoader(new WXLogPrinter);
	TextLoader* textLoaderPtr = nullptr;
	TextParser parser(textLoader.getSeparator(), new WXLogPrinter);
	for (auto& name : filesNames)
	{
		try
		{
			std::string fileNameStr = (inputFolderTextField->GetLabelText() + "\\" + name).ToStdString();
			std::wregex designerPattern(LR"(^GCC\-(\w{3})\-DDD\-)");

			std::wsmatch match;
			if (std::regex_search(name.ToStdWstring(), match, designerPattern) && match[1] == L"FGG") {
				textLoaderPtr = &dotNetTextLoader;
			}
			else {
				textLoaderPtr = &textLoader;
			}
			textLoaderPtr->loadFile(fileNameStr);
		}
		catch (std::exception& ex)
		{
			wxLogMessage("Во время попытки чтения файла %s возникла ошибка:\n %s", wxString(name), wxString(ex.what()));
			filesWithReadingErrors.emplace_back(name);
			exceptionsMessages.push_back("Во время попытки чтения файла " + name + " возникла ошибка: " + ex.what());
			continue;
		}
		try
		{
			TextParser::ResultState resultState = parser.parse(*textLoaderPtr);
		}
		catch (std::exception ex) 
		{
			wxLogMessage("Во время попытки парсинга файла %s возникла ошибка:\n %s", wxString(name), wxString(ex.what()));
			wxString errorMessageStr = ex.what();
			size_t startIndex = errorMessageStr.find(L", значение: ");
			wxString errorValueStr = errorMessageStr;
			if (startIndex != std::wstring::npos) 
			{
				errorValueStr = errorMessageStr.substr(startIndex + 11, errorMessageStr.size() - 1);
				errorMessageStr.erase(startIndex);
			}
			std::replace(errorMessageStr.begin(), errorMessageStr.end(), ':', '-');
			errorMessageStr.insert(errorMessageStr.find_first_of('-'), ' ');
			filesWithParsingErrors.insert(std::pair(errorMessageStr, ParsingErrorInfo(name, errorValueStr)));
			exceptionsMessages.push_back("Во время попытки парсинга файла " + name + " возникла ошибка: " + ex.what());
			continue;
		}
	}

	for(auto& name : filesWithReadingErrors) 
	{
		moveFile(name, L"Нечитаемые файлы");
	}

	for (auto& [error, info] : filesWithParsingErrors)
	{
		std::filesystem::path folderPath = L"Файлы с ошибками\\" + error.ToStdWstring();
		moveFile(info.fileName, folderPath.wstring());
		saveErrorInfoToFile(L"Информация о ошибках.txt", folderPath.wstring(), info);
	}

	if (filesWithParsingErrors.size() != 0)
	{
		wxLogMessage("---------------------------------------------------");
		wxLogMessage("Таблица не была создана, т.к. были обнаружены файлы с ошибками парсинга,\n файлы с ошибками были перемещены в подпапку, повторите попытку парсинга!");
		wxLogMessage("---------------------------------------------------");
		return;
	}

	TableWriter tableWriter(parser.getDrawings(), new WXLogPrinter);
	tableWriter.createNewTableFile(outputFolderTextField->GetLabelText().ToStdWstring());
	tableWriter.writeTable();

	SetStatusText("Таблица " + tableWriter.getFileName() + " создана!");
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.SetName(_("Bom generator"));
	info.SetVersion(_("1.8.0"));
	info.SetDescription(_(aboutStr));
	info.SetCopyright("(C) 2025 Dmitriy Zelenev <Dmitry22_s@cc7.cn>");
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
		wxMessageBox("Путь к " + errMessage + " не выбран или указан не верно!", "Выберите путь к папке", wxOK | wxICON_EXCLAMATION);
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
		wxMessageBox("В папке отсутствуют файлы или путь к папке указан не верно!", "Файлы не найдены", wxOK | wxICON_EXCLAMATION);
		return;
	}
	while (contains) {
		if (fileName.ends_with(".dwg")) {
			filesNames.push_back(fileName);
		}
		contains = dir.GetNext(&fileName);
	}

	if (filesNames.size() == 0) {
		wxMessageBox("В папке отсутствуют файлы dwg!", "Файлы не найдены", wxOK | wxICON_EXCLAMATION);
	}
}

void MainWindow::moveFile(const wxString& fileName, const std::wstring& destinationFolderName)
{
	namespace fs = std::filesystem;

	fs::path oldPath = inputFolderTextField->GetLabelText().ToStdWstring() + L'\\' + fileName.ToStdWstring();
	fs::path newFolderPath = inputFolderTextField->GetLabelText().ToStdWstring() + L'\\' + destinationFolderName;
	fs::path newPath = newFolderPath / fileName.ToStdWstring();

	try
	{
		if (!fs::exists(newFolderPath))
		{
			fs::create_directories(newFolderPath);
		}

		fs::rename(oldPath, newPath);
	}
	catch (const fs::filesystem_error& ex) {
		wxLogMessage("Ошибка при перемещении файла %s : %s", fileName, ex.what());
	}
}

void MainWindow::saveErrorInfoToFile(const wxString& fileName, const std::wstring& destinationFolderName, ParsingErrorInfo errorInfo)
{
	namespace fs = std::filesystem;

	fs::path fullFolderPath = inputFolderTextField->GetLabelText().ToStdWstring() + L'\\' + destinationFolderName;

	fs::path filePath = fullFolderPath / fileName.ToStdWstring();
	wxFile logFile(filePath.wstring(), wxFile::write_append);
	wxString line = wxString::Format("Файл: %s, значение: %s\n", errorInfo.fileName, errorInfo.errorValue);
	logFile.Write(line);
	logFile.Close();

	//std::filesystem::path filePath = fullFolderPath / fileName.ToStdWstring();
	//std::wofstream file(filePath, std::ios::app);
	//file << L"Файл: " << errorInfo.fileName << L", значение: " << errorInfo.errorValue << '\n';
	//file.close();
}
