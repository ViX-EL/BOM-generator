#include "DWGTextLoader.h"
#include "IMessagePrinter.h"
#include "StringConvert.h"
#include "DWGDotNetTextLoader.h"

#include "dwg.h"
#include "dwg_api.h"

//#include <boost/log/trivial.hpp>
#include <wx/log.h> 
#include <string>
#include <cassert>

DWGTextLoader::DWGTextLoader(IMessagePrinter* printer) : TextLoader(printer)
{
	
}

const char* DWGTextLoader::process_text(Dwg_Object* obj)
{
	if (!obj) return nullptr;

	char* text_value{ nullptr };
	int isnew{ 0 };
	if(obj->type == DWG_TYPE_TEXT)
	{
		Dwg_Entity_TEXT* textEntity = obj->tio.entity->tio.TEXT;
		if (textEntity && textEntity->text_value) {
			dwg_dynapi_entity_utf8text(textEntity, "TEXT", "text_value", &text_value, &isnew, NULL);
		}
	}
	else if (obj->type == DWG_TYPE_MTEXT) 
	{
		Dwg_Entity_MTEXT* mtextEntity = obj->tio.entity->tio.MTEXT;
		if (mtextEntity && mtextEntity->text) {
			dwg_dynapi_entity_utf8text(mtextEntity, "MTEXT", "text", &text_value, &isnew, NULL);
		}
	}
	return text_value;
}

int DWGTextLoader::loadFile(const std::string& filePath)
{
	text.clear();
	setFilePath(filePath);
	setFileName(getFilePath());

	//printer->printText(L"Начало чтения файла " + getFileName(), L"[Чтение]");
	wxLogMessage(L"[Чтение] Начало чтения файла %s", getFileName());

	Dwg_Data dwg;

	memset(&dwg, 0, sizeof(Dwg_Data));
	int success = dwg_read_file(filePath.c_str(), &dwg);
	if (!(success < DWG_ERR_CRITICAL))
	{
		printer->printError(L"Сломанный файл " + utf8_decode(filePath) + L'!');
		throw std::exception("Wrong file!");
	}

	int found{ 0 };
	for (BITCODE_BL i = 0; i < dwg.num_objects; i++)
	{
		Dwg_Object* obj = &dwg.object[i];
		if (obj->type == DWG_TYPE_TEXT || obj->type == DWG_TYPE_MTEXT)
		{
			found++;
			text.append(separator + utf8_decode(process_text(obj)));
		}
	}

	//for (unsigned i = 0; i < dwg.num_objects; i++) 
	//{
	//	Dwg_Object* obj = &dwg.object[i];
	//	if (obj->type == DWG_TYPE_INSERT) 
	//	{
	//		Dwg_Entity_INSERT* insert = obj->tio.entity->tio.INSERT;

	//		// Получаем имя блока
	//		const char* block_name = insert->block_header->obj->name;

	//		// Ищем определение блока по имени
	//		for (unsigned j = 0; j < dwg.num_objects; j++) 
	//		{
	//			Dwg_Object* block_obj = &dwg.object[j];
	//			if (block_obj->type == DWG_TYPE_BLOCK_HEADER) 
	//			{
	//				Dwg_Object_BLOCK_HEADER* block = block_obj->tio.object->tio.BLOCK_HEADER;
	//				if (strcmp(block->name, block_name) == 0) 
	//				{
	//					// Обходим объекты внутри блока
	//					for (unsigned k = 0; k < block->num_owned; k++) 
	//					{
	//						_dwg_object_ref* objRef = block->entities[k];
	//						if (objRef->obj->type == DWG_TYPE_TEXT || objRef->obj->type == DWG_TYPE_MTEXT)
	//						{
	//							found++;
	//							text.append(separator + utf8_decode(process_text(objRef->obj)));
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	text = text + separator;

	if (!found)
	{
		printer->printError(L"Текст не найден в файле " + utf8_decode(filePath) + L'!');
		throw std::exception("Text not found!");
	}

	dwg_free(&dwg);

	wxLogMessage(L"[Чтение] Конец чтения файла %s", getFileName());
	//printer->printText(L"Конец чтения файла " + getFileName(), L"[Чтение]");

	return success;
}
