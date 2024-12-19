#pragma once

#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include "BaseTextParser.h"
#include "IMessagePrinter.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "DrawingPage.h"

class TextParser
{
public:
	TextParser(const std::wstring& text, wchar_t separator, IMessagePrinter* printer);
	void parse(const std::wstring& fileName);
	const std::vector<Drawing>& getDrawings() const;

private:
	enum class DesignerIndex
	{
		LGN, ASP, MEN, IOT, NAG, PTE
	};
	DesignerIndex designerIdx = DesignerIndex::LGN;

	const std::vector<std::wstring> designersNames {L"LGN", L"ASP", L"MEN", L"IOT", L"NAG", L"PTE"};

	const std::wstring* text{ nullptr };
	std::vector<Drawing> drawings;
	wchar_t separator{'^'};

	std::unique_ptr<IMessagePrinter> printer;

	std::shared_ptr<BaseTextParser> currentParser;

	std::shared_ptr<BaseTextParser> parsers[6];
};

#endif // TEXTPARSER_H

