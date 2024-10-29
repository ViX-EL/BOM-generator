#include "DWGTextLoader.h"

#include "dwg.h"
#include "dwg_api.h"

#include "IMessagePrinter.h"
#include "StringConvert.h"

#include <boost/log/trivial.hpp>
#include <wx/log.h> 
#include <string>
#include <iostream>
#include <vector>

DWGTextLoader::DWGTextLoader(IMessagePrinter* printer) : TextLoader(printer)
{
	
}

int DWGTextLoader::loadFile(const std::string& filePath)
{
	/*BOOST_LOG_TRIVIAL(trace) << "Начало чтения файла " << filename;*/

	text.clear();
	setFilePath(filePath);
	setFileName(getFilePath());

	wxLogMessage("[Чтение] Начало чтения файла %s", wxString(getFileName()));

	Dwg_Data dwg;

	memset(&dwg, 0, sizeof(Dwg_Data));
	int success = dwg_read_file(filePath.c_str(), &dwg);
	if (!(success < DWG_ERR_CRITICAL)) {
		printer->printError(L"Wrong file!");
		return success;
	}

	char* text_value{ nullptr };
	int found{ 0 };
	int isnew{ 0 };
	std::wstring text2;
	for (BITCODE_BL i = 0; i < dwg.num_objects; i++)
	{
		if (dwg.object[i].type == DWG_TYPE_TEXT)
		{
			Dwg_Entity_TEXT* _obj = dwg.object[i].tio.entity->tio.TEXT;
			found++;
			dwg_dynapi_entity_utf8text(_obj, "TEXT", "text_value", &text_value, &isnew, NULL);
			text.append(separator + utf8_decode(text_value));
		}
	}

	text = text + separator;

	if (!found) {
		printer->printError(L"Text not found!");
		return 0;
	}

	dwg_free(&dwg);

	//BOOST_LOG_TRIVIAL(trace) << "Конец чтения файла " << filename;
	wxLogMessage("[Чтение] Конец чтения файла %s", wxString(getFileName()));

	return success;
}
