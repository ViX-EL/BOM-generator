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
	wxString aboutStr{ "\nПрограмма читает множество dwg файлов изометрических чертежей "
		"трубопроводов из входной папки и преобразует прочитанные данные в "
		"одну xlsx (файл Excel) таблицу в выходной папке.\n\nДля создания Excel таблицы перечня "
		"материалов трубопроводов выберите входную и выходную "
		"папки и нажмите кнопку \"создать Excel таблицу\". Пока доступна обработка файлов "
		"от проектировщиков \"LGN\", \"ASP\", \"IOT\", \"NAG\". \n\nВо время обработки файлов могут "
		"встречаться ошибки. Сообщайте об ошибках на Dmitry22_s@cc7.cn, в письме указывайте тип "
		"ошибки - файл не обрабатывается / ошибка в столбцах / другое, перечислите наименования "
		"проблемных столбцов, если они есть, а так же сообщите имя файла и приложите скриншот с ошибкой."};
};

