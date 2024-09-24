#include "TextParser.h"

#include <regex>
#include <vector>

TextParser::TextParser(const std::wstring& text, wchar_t separator,IMessagePrinter* printer) : 
	text(&text), separator(separator), printer(printer)
{
	
}

const std::vector<int>& TextParser::getComponentsCountPerList() const
{
	return componentsCountPerList;
}

void TextParser::parse(const std::wstring& fileName)
{
	std::wregex cipherDocumentPattern(LR"(GCC-\w{3}-DDD)");

	std::wstring currentDesignerStr;

	size_t fileNameStartIdx = fileName.find_last_of(L'\\');
	std::wstring newfileNameStr = fileName;
	if (fileNameStartIdx != std::wstring::npos) {
		newfileNameStr = fileName.substr(fileNameStartIdx + 1);
	}

	if (std::regex_search(newfileNameStr, cipherDocumentPattern)) {
		currentDesignerStr = newfileNameStr.substr(4, 3);
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
		if (!parsers[index]) {
			parsers[index] = std::make_shared<TextParserLGN>(*text, columns, componentsCountPerList, separator);
		}
		currentParser = parsers[index];
		break;
	case DesignerIndex::ASP:
		if (!parsers[index]) {
			parsers[index] = std::make_shared<TextParserASP>(*text, columns, componentsCountPerList, separator);
		}
		currentParser = parsers[index];
		break;
	case DesignerIndex::MEN:
		if (!parsers[index])
		{
			printer->printError(L"Обработка файлов проектировщика " + currentDesignerStr +
				L" находится в разработке!");
			parsers[index] = std::make_shared<TextParserMEN>(*text, columns, componentsCountPerList, separator);
		}
		currentParser = parsers[index];
		break;
	case DesignerIndex::IOT:
		if (!parsers[index]) {
			parsers[index] = std::make_shared<TextParserIOT>(*text, columns, componentsCountPerList, separator);
		}
		currentParser = parsers[index];
		break;
	case DesignerIndex::NAG:
		if (!parsers[index]) {
			parsers[index] = std::make_shared<TextParserNAG>(*text, columns, componentsCountPerList, separator);
		}
		currentParser = parsers[index];
		break;
	default:
		printer->printError(L"Обработка файлов проектировщика " + currentDesignerStr +
			L" пока не поддерживается приложением!");
		return;
	}

	currentParser->parse(newfileNameStr);
}