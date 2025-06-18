#include "TextParser.h"

#include "TextParserLGN.h"
#include "TextParserASP.h"
#include "TextParserIOT.h"
#include "TextParserNAG.h"
#include "TextParserPTE.h"
#include "TextParserFGG.h"
#include "TextParserSEC.h"
#include "TextParserHMS.h"
#include "TextParserOZN.h"
#include "IMessagePrinter.h"
#include "StringConvert.h"
#include "StringUtilities.h"
#include "TextLoader.h"

#include <wx/log.h> 
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <regex>


TextParser::TextParser(wchar_t separator, IMessagePrinter* printer) : separator(separator), printer(printer)
{
	parsers.reserve(designersNames.size());
	for (size_t i = 0; i < designersNames.size(); i++) {
		parsers.emplace_back(nullptr);
	}
}

template <baseOfBaseTextParser T>
void TextParser::createParser(int designerIndex) 
{
	if (!parsers[designerIndex]) {
		parsers[designerIndex] = std::make_shared<T>(*text, separator);
	}
	currentParser = parsers[designerIndex];
}

template void TextParser::createParser<TextParserLGN>(int designerIndex);
template void TextParser::createParser<TextParserASP>(int designerIndex);
template void TextParser::createParser<TextParserIOT>(int designerIndex);
template void TextParser::createParser<TextParserNAG>(int designerIndex);
template void TextParser::createParser<TextParserPTE>(int designerIndex);
template void TextParser::createParser<TextParserFGG>(int designerIndex);
template void TextParser::createParser<TextParserSEC>(int designerIndex);
template void TextParser::createParser<TextParserHMS>(int designerIndex);
template void TextParser::createParser<TextParserOZN>(int designerIndex);

TextParser::ResultState TextParser::parse(const TextLoader& textLoader)
{
	text = &textLoader.getText();

	std::wregex cipherDocumentPattern(StringUtilities::getRegex(LR"(GCC-\w{3}-DDD-\d+-\d+-\d+-\w+-\w+-\d+)"));

	std::wstring currentDesignerStr;

	if (std::regex_search(textLoader.getFileName(), cipherDocumentPattern)) {
		currentDesignerStr = textLoader.getFileName().substr(textLoader.getFileName().find(L"GCC-") + 4, 3);
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

	switch (designerIdx)
	{
	case DesignerIndex::LGN:
		createParser<TextParserLGN>(index);
		break;
	case DesignerIndex::ASP:
	case DesignerIndex::AFT:
		if (index == static_cast<int>(DesignerIndex::AFT)) {
			index = static_cast<int>(DesignerIndex::ASP);
		}
		createParser<TextParserASP>(index);
		break;
	case DesignerIndex::IOT:
		createParser<TextParserIOT>(index);
		break;
	case DesignerIndex::NAG:
		createParser<TextParserNAG>(index);
		break;
	case DesignerIndex::PTE:
		createParser<TextParserPTE>(index);
		break;
	case DesignerIndex::FGG:
		createParser<TextParserFGG>(index);
		break;
	case DesignerIndex::SEC:
		createParser<TextParserSEC>(index);
		break;
	case DesignerIndex::HMS:
		createParser<TextParserHMS>(index);
		break;
	case DesignerIndex::OZN:
		createParser<TextParserOZN>(index);
		break;
	default:
		if (currentDesignerStr == L"")
		{
			printer->printError(L"В файле " + textLoader.getFileName() + L" имя проектировщика не найдено!");
			return ResultState::FAILED;
		}
		printer->printError(L"Обработка файлов проектировщика " + currentDesignerStr + L" пока не поддерживается приложением!");
		return ResultState::NOT_SUPPORTED;
	}

	try 
	{
		wxLogMessage(L"[Парсинг] Начало парсинга файла %s", textLoader.getFileName());
		//printer->printText(L"Начало парсинга файла " + textLoader.getFileName(), L"[Парсинг]");

		size_t sizeBeforeParsing = drawings.size();
		if (currentParser != nullptr) {
			currentParser->parse(textLoader.getFileName(), drawings);
		}

		if(drawings.size() == sizeBeforeParsing)
		{
			wxLogMessage(L"[Запись] В файле %s нет записываемых листов!", textLoader.getFileName());
			//printer->printText(L"[Запись] В файле " + textLoader.getFileName() + L" нет записываемых листов!", L"[Запись]");
		}

		//printer->printText(L"Конец парсинга файла " + textLoader.getFileName() + L' ' + std::to_wstring(drawings.size()), L"[Парсинг]");
		int parsedDrawingsNumber = drawings.size();
		wxLogMessage(L"[Парсинг] Конец парсинга файла %s %d", textLoader.getFileName(), parsedDrawingsNumber);
		return ResultState::SUCCESS;
	}
	catch (const std::exception& ex) 
	{
		std::string message = ex.what();
		throw std::exception(("Ошибка парсинга: " + message).c_str());
	}
}

const std::vector<Drawing>& TextParser::getDrawings() const
{
	return drawings;
}
