//Используемые сторонние библиотеки:
//https://github.com/LibreDWG/libredwg
//https://github.com/jmcnamara/libxlsxwriter
//https://github.com/wxWidgets/wxWidgets

#include <iostream>
#include <string>

#include <wx/wx.h>
#include "DialogPrinter.h"
#include "DOCXPrinter.h"
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

	//ASP%ISO%

	std::string fileName = "GCC-PGZ-DDD-00011-06-1100-TK-ISO-00001_03_2_source" ".dwg";

	processingOneFile(fileName, LOAD);

	return true;
}

void DrawingsParserApp::processingOneFile(std::string& fileName, unsigned int state)
{
	if (state == NONE || (state != LOAD && state != LOAD_PARSE && state != LOAD_PARSE_WRITE)) {
		return;
	}

	DWGTextLoader textLoader(new DOCXPrinter);
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
		if (!parser.getColumns().empty())
		{
			TableWriter tableWriter(parser.getColumns(), parser.getComponentsCountPerList());
			tableWriter.createNewTableFile(".\\");
			tableWriter.writeTable();
		}
	}
}
