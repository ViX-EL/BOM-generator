//Используемые сторонние библиотеки:
//https://github.com/LibreDWG/libredwg
//https://github.com/jmcnamara/libxlsxwriter
//https://github.com/wxWidgets/wxWidgets

#include <iostream>
#include <string>

#include <wx/wx.h>
#include <wx/log.h>

#include "DialogPrinter.h"
#include "DWGTextLoader.h"
#include "PDFTextLoader.h"
#include "TextParser.h"
#include "TableWriter.h"
#include "MainWindow.h"
#include "BOMGeneratorApp.h"

IMPLEMENT_APP(DrawingsParserApp);

bool DrawingsParserApp::OnInit()
{
	MainWindow* window = new MainWindow("BOM generator", wxSize(440, 220));
	window->SetBackgroundColour(*wxWHITE);
	window->Show(true);

	//setlocale(LC_ALL, "Russian");

	std::string fileName = "12600-13-3312-570-P12-0037-ACB2B05SN61-03" ".dwg";

	//processingOneFile(fileName, LOAD_PARSE_WRITE);
	processingOneFile(fileName, LOAD);

	return true;
}

void DrawingsParserApp::processingOneFile(std::string& fileName, unsigned int state)
{
	if (state == NONE || (state != LOAD && state != LOAD_PARSE && state != LOAD_PARSE_WRITE)) {
		return;
	}

	DWGTextLoader textLoader(new DialogPrinter);
	DWGDotNetTextLoader dotNetTextLoader(new DialogPrinter);
	TextLoader* textLoaderPtr = nullptr;
	if (state & LOAD || state & LOAD_PARSE || state & LOAD_PARSE_WRITE)
	{
		try 
		{
			std::regex designerPattern(R"(^GCC\-(\w{3})\-DDD\-)");
			std::smatch match;
			if (std::regex_search(fileName, match, designerPattern) && match[1] == "FGG") {
				textLoaderPtr = &dotNetTextLoader;
			}
			else {
				textLoaderPtr = &textLoader;
			}
			textLoaderPtr->loadFile(fileName);
			if (state & LOAD)
			{
				textLoaderPtr->printText(textLoaderPtr->getFileName());
			}
		}
		catch (std::exception& ex)
		{
			wxLogMessage("[Чтение] Ошибка чтения файла %s :\n%s", fileName, ex.what());
		}
	}

	TextParser parser(textLoader.getSeparator(), new DialogPrinter);
	if (state & LOAD_PARSE || state & LOAD_PARSE_WRITE) {
		parser.parse(*textLoaderPtr);
	}

	if (state & LOAD_PARSE_WRITE) 
	{
		if (parser.getDrawings().size() != 0)
		{
			TableWriter tableWriter(parser.getDrawings(), new DialogPrinter);
			tableWriter.createNewTableFile(L".\\");
			tableWriter.writeTable();
		}
		//else {
		//	wxLogMessage("[Запись] Нет записываемых листов в файле %s", fileName);
		//}
	}
}

DrawingsParserApp::~DrawingsParserApp()
{

}
