#pragma once

#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include "BaseTextParser.h"
#include "TextParserLGN.h"
#include "TextParserASP.h"
#include "TextParserMEN.h"
#include "TextParserIOT.h"
#include "TextParserNAG.h"
#include "IMessagePrinter.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

class TextParser
{
public:
	TextParser(const std::wstring& text, wchar_t separator, IMessagePrinter* printer);
	void parse(const std::wstring& fileName);
	const BaseTextParser::Columns& getColumns() const { return columns; }
	const std::vector<int>& getComponentsCountPerList() const;

private:
	enum class DesignerIndex
	{
		LGN, ASP, MEN, IOT, NAG, PTE
	};
	DesignerIndex designerIdx = DesignerIndex::LGN;

	const std::vector<std::wstring> designersNames {L"LGN", L"ASP", L"MEN", L"IOT", L"NAG", L"PTE"};

	const std::wstring* text{ nullptr };
	BaseTextParser::Columns columns;
	std::vector<int> componentsCountPerList;
	wchar_t separator{'^'};

	std::unique_ptr<IMessagePrinter> printer;

	std::shared_ptr<BaseTextParser> currentParser;

	std::shared_ptr<BaseTextParser> parsers[6];
};

#endif // TEXTPARSER_H

