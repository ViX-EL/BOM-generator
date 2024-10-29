//Используемые сторонние библиотеки:
//https://github.com/LibreDWG/libredwg
//https://github.com/jmcnamara/libxlsxwriter
//https://github.com/wxWidgets/wxWidgets

#include <iostream>
#include <string>

#include <wx/wx.h>
#include <wx/log.h>

//#include <boost/log/core.hpp>
//#include <boost/log/trivial.hpp>
//#include <boost/log/expressions.hpp>
//#include <boost/log/utility/setup/file.hpp>
//#include <boost/log/utility/setup/common_attributes.hpp>

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

	//ASP%ISO%

	//AllocConsole();

	//logger = new wxLogStream();
	//wxLog::SetActiveTarget(logger);

	//initLogging();

	std::string fileName = "16150-11-2200_031-W12-0013-ESB1B01BN-01_Sht_4" ".dwg";

	processingOneFile(fileName, LOAD);

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
		if (!parser.getColumns().empty())
		{
			TableWriter tableWriter(parser.getColumns(), parser.getComponentsCountPerList(), new DialogPrinter);
			tableWriter.createNewTableFile(".\\");
			tableWriter.writeTable();
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
