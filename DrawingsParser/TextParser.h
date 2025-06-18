#pragma once

#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include "BaseTextParser.h"
#include "IMessagePrinter.h"
#include "TextLoader.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

template <typename T>
concept baseOfBaseTextParser = std::is_base_of<BaseTextParser, T>::value;

class TextParser
{
public:
	enum class ResultState
	{
		SUCCESS,
		FAILED,
		NOT_SUPPORTED
	};
	TextParser(wchar_t separator, IMessagePrinter* printer);
	ResultState parse(const TextLoader& textLoader);
	const std::vector<Drawing>& getDrawings() const;

private:
	enum class DesignerIndex
	{
		LGN, ASP, IOT, NAG, PTE, AFT, FGG, SEC, HMS, OZN
	};
	DesignerIndex designerIdx = static_cast<DesignerIndex>(-1);

	const std::vector<std::wstring> designersNames {L"LGN", L"ASP", L"IOT", L"NAG", L"PTE", L"AFT", L"FGG", L"SEC", L"HMS", L"OZN" };

	const std::wstring* text{ nullptr };
	std::vector<Drawing> drawings;
	wchar_t separator{'^'};

	std::unique_ptr<IMessagePrinter> printer;

	std::shared_ptr<BaseTextParser> currentParser;

	std::vector<std::shared_ptr<BaseTextParser>> parsers;

	template <baseOfBaseTextParser T>
	void createParser(int designerIndex);
};

#endif // TEXTPARSER_H

