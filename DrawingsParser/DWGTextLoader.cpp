#include "DWGTextLoader.h"

#include "dwg.h"
#include "dwg_api.h"

#include "IMessagePrinter.h"
#include "StringConvert.h"

#include <string>
#include <iostream>
#include <vector>

DWGTextLoader::DWGTextLoader(IMessagePrinter* printer) : TextLoader(printer)
{
	
}

int DWGTextLoader::loadFile(const std::string& filename)
{
	text.clear();
	this->filename = utf8_decode(filename);

	Dwg_Data dwg;

	memset(&dwg, 0, sizeof(Dwg_Data));
	int success = dwg_read_file(filename.c_str(), &dwg);
	if (!(success < DWG_ERR_CRITICAL)) {
		printer->printError(L"Wrong file!");
		return success;
	}

	char* text_value{ nullptr };
	int found{ 0 };
	int isnew{ 0 };
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

	if (!found) {
		printer->printError(L"Text not found!");
		return 0;
	}

	dwg_free(&dwg);
	return success;
}
