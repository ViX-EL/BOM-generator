#include "TextParser.h"

#include "TextParserLGN.h"
#include "TextParserASP.h"
#include "TextParserMEN.h"
#include "TextParserIOT.h"
#include "TextParserNAG.h"
#include "TextParserPTE.h"
#include "IMessagePrinter.h"
#include "StringConvert.h"
#include "StringUtilities.h"

#include <wx/log.h> 
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <regex>


TextParser::TextParser(const std::wstring& text, wchar_t separator,IMessagePrinter* printer) : 
	text(&text), separator(separator), printer(printer)
{
	
}

void TextParser::parse(const std::wstring& fileName)
{
	std::wregex cipherDocumentPattern(StringUtilities::getRegex(LR"(GCC-\w{3}-DDD-\d+-\d+-\d+-\w+-\w+-\d+)"));

	std::wstring currentDesignerStr;

	if (std::regex_search(fileName, cipherDocumentPattern)) {
		currentDesignerStr = fileName.substr(fileName.find(L"GCC-") + 4, 3);
	}
	else
	{
		std::wsmatch match;
		if (std::regex_search(*text, match, cipherDocumentPattern)) {
			currentDesignerStr = match.str(0).substr(4, 3);
		}
	}

	size_t index = 0;
	for (size_t i = 0; i < designersNames.size(); i++)
	{
		if (currentDesignerStr == designersNames[i])
		{
			designerIdx = static_cast<DesignerIndex>(i);
			index = i;
			break;
		}
	}

	switch (designerIdx) //TODO переписать все конструкторы текстовых парсеров
	{
	case DesignerIndex::LGN:
		if (!parsers[index]) {
			parsers[index] = std::make_shared<TextParserLGN>(*text, separator);
		}
		currentParser = parsers[index];
		break;
	case DesignerIndex::ASP:
		if (!parsers[index]) {
			parsers[index] = std::make_shared<TextParserASP>(*text, separator);
		}
		currentParser = parsers[index];
		break;
	case DesignerIndex::MEN:
		if (!parsers[index])
		{
			printer->printError(L"Обработка файлов проектировщика " + currentDesignerStr + L" находится в разработке!");
			/*parsers[index] = std::make_shared<TextParserMEN>(*text, columns, componentsCountPerList, separator);*/
		}
		currentParser = parsers[index];
		break;
	case DesignerIndex::IOT:
		if (!parsers[index]) {
			parsers[index] = std::make_shared<TextParserIOT>(*text, separator);
		}
		currentParser = parsers[index];
		break;
	case DesignerIndex::NAG:
		if (!parsers[index]) {
			parsers[index] = std::make_shared<TextParserNAG>(*text, separator);
		}
		currentParser = parsers[index];
		break;
	case DesignerIndex::PTE:
		if (!parsers[index])
		{
			printer->printError(L"Обработка файлов проектировщика " + currentDesignerStr + L" находится в разработке!");
		/*	parsers[index] = std::make_shared<TextParserPTE>(*text, columns, componentsCountPerList, separator);*/
		}
		currentParser = parsers[index];
		break;
	default:
		if (currentDesignerStr == L"")
		{
			printer->printError(L"В файле " + fileName + L" имя проектировщика не найдено!");
			return;
		}
		printer->printError(L"Обработка файлов проектировщика " + currentDesignerStr + L" пока не поддерживается приложением!");
		return;
	}

	try {
		wxLogMessage("[Парсинг] Начало парсинга файла %s", wxString(fileName));

		size_t sizeBeforeParsing = drawings.size();
		if (currentParser != nullptr) {
			currentParser->parse(fileName, drawings);
		}

		if(drawings.size() == sizeBeforeParsing)
		{
			wxLogMessage("[Запись] В файле %s нет записываемых листов!", wxString(fileName));
		}

		wxLogMessage("[Парсинг] Конец парсинга файла %s", wxString(fileName + L" " + std::to_wstring(drawings.size())));
	}
	catch (const std::exception& ex) {
		printer->printError(L"Ошибка парсинга: " + utf8_decode(ex.what()) + L" \nВ файле " + fileName);
	}
}

const std::vector<Drawing>& TextParser::getDrawings() const
{
	return drawings;
}
