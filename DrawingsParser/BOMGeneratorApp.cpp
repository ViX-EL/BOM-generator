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

	std::string fileName = "16150-11-2200_031-COE-0001-ESB1B01BN-02_Sht_1" ".dwg";

	//processingOneFile(fileName, LOAD_PARSE_WRITE);
	//processingOneFile(fileName, LOAD);

	return true;
}

void DrawingsParserApp::processingOneFile(std::string& fileName, unsigned int state)
{
	if (state == NONE || (state != LOAD && state != LOAD_PARSE && state != LOAD_PARSE_WRITE)) {
		return;
	}

	DWGTextLoader textLoader(new DialogPrinter);
	if (state & LOAD || state & LOAD_PARSE || state & LOAD_PARSE_WRITE)
	{
		textLoader.loadFile(fileName);
		if (state & LOAD) {
			textLoader.printText(textLoader.getFileName());
		}
	}

	TextParser parser(textLoader.getText(), textLoader.getSeparator(), new DialogPrinter);
	if (state & LOAD_PARSE || state & LOAD_PARSE_WRITE) {
		parser.parse(textLoader.getFileName());
	}

	if (state & LOAD_PARSE_WRITE) 
	{
		if (parser.getDrawings().size() != 0)
		{
			TableWriter tableWriter(parser.getDrawings(), new DialogPrinter);
			tableWriter.createNewTableFile(L".\\");
			tableWriter.writeTable();
		}
		else {
			wxLogMessage("[Запись] Нет записываемых листов в файле %s", fileName);
		}
	}
}

DrawingsParserApp::~DrawingsParserApp()
{

}
