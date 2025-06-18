#pragma once

#ifndef MainWindow_h__
#define MainWindow_h__

#include <wx/wx.h>
#include <wx/log.h> 
#include <vector>
#include <string>
#include <memory>

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxSize& size);

	void OnSelectInputFolder(wxCommandEvent& event);
	void OnSelectOutputFolder(wxCommandEvent& event);
	void OnCreateXlsxTable(wxCommandEvent& event);

	~MainWindow();

private:

	struct ParsingErrorInfo
	{
		wxString fileName;
		wxString errorValue;
	};

	std::unique_ptr<wxLog> logger;

	wxTextCtrl* inputFolderTextField;
	wxTextCtrl* outputFolderTextField;
	std::vector<wxString> filesNames;

	wxString aboutStr{ "\nПрограмма читает множество dwg файлов изометрических чертежей "
		"трубопроводов из входной папки и преобразует прочитанные данные в "
		"одну xlsx (файл Excel) таблицу в выходной папке.\n\nДля создания Excel таблицы перечня "
		"материалов трубопроводов выберите входную и выходную "
		"папки и нажмите кнопку \"создать Excel таблицу\". Создание таблиц доступно для файлов"
		"от проектировщиков \"LGN\", \"ASP\", \"IOT\", \"NAG\", \"PTE\". \n\nВо время обработки файлов могут "
		"встречаться ошибки. Сообщайте об ошибках на почту - Dmitry22_s@cc7.cn или Teams - Dmitriy Zelenev, в письме указывайте тип "
		"ошибки - файл не обрабатывается / ошибка в столбцах / другое, перечислите наименования "
		"проблемных столбцов, если они есть, а так же сообщите имя файла и приложите снимок экрана с ошибкой." };

	void OnAbout(wxCommandEvent& event);

	void selectPath(wxTextCtrl* textField);
	bool isPathCorrect(wxTextCtrl* textField, const wxString& errMessage);
	void getFileNames();

	void moveFile(const wxString& fileName, const std::wstring& destinationFolderName);
	void saveErrorInfoToFile(const wxString& fileName, const std::wstring& destinationFolderName, ParsingErrorInfo errorInfo);
};

#endif // MainWindow_h__

