//Используемые сторонние библиотеки:
//https://github.com/LibreDWG/libredwg
//https://github.com/jmcnamara/libxlsxwriter
//https://github.com/wxWidgets/wxWidgets

#include <iostream>
#include <string>

#include <wx/wx.h>
#include <wx/log.h>

#include "DialogPrinter.h"
//#include "DOCXPrinter.h"
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

	std::string fileName = "16150-11-2200_005-IA-0711-GCB2B01BN-02_Sht_1" ".dwg";

	processingOneFile(fileName, NONE);

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
		if (parser.getComponentsCountPerList().size() != 0)
		{
			TableWriter tableWriter(parser.getColumns(), parser.getComponentsCountPerList(), new DialogPrinter);
			tableWriter.createNewTableFile(".\\");
			tableWriter.writeTable();
		}
		else {
			wxLogMessage("[Запись] Нет записываемых листов в файле %s", fileName);
		}
	}
}

DrawingsParserApp::~DrawingsParserApp()
{
	//wxLog::SetActiveTarget(nullptr);
	//delete logger;
}

//void DrawingsParserApp::initLogging()
//{
//	namespace logging = boost::log;
//	namespace keywords = boost::log::keywords;
//
//	logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");
//
//	//logging::core::get()->set_filter
//	//(
//	//	logging::trivial::severity >= logging::trivial::trace
//	//);
//
//	logging::add_file_log
//	(
//		keywords::file_name = "GeneratorLog.log",
//		keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
//	);
//
//	logging::add_common_attributes();
//}
