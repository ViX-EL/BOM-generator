#pragma once

#ifndef TextParserNAG_h__
#define TextParserNAG_h__

#include "BaseTextParser.h"
#include "DrawingPage.h"
#include "ValuesCheker.h"

#include <initializer_list>
#include <string>
#include <vector>

class TextParserNAG : public BaseTextParser
{
private:

	bool currentListEmpty = false;

	bool readComponentNumber() override;
	void readTablePartData();

	void writeValueOfTwoSubStr(bool(DrawingPage::* trySetFunction)(const std::wstring&, ValuesCheker::Type), std::initializer_list<std::wstring> ifEqualStr,
		const std::wstring& firstStr = L"", ValuesCheker::Type checkType = ValuesCheker::getType());
	void reset() override;
	bool readComponent() override;
	bool isEndOfComponent(const std::wstring& stringAfterComponent) const;
public:
	TextParserNAG(const std::wstring& text, wchar_t separator);
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
};

#endif // TextParserNAG_h__

