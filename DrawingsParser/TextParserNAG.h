#pragma once
#include "BaseTextParser.h"
#include "DrawingPage.h"
#include <initializer_list>
#include <string>
#include <vector>

class TextParserNAG : public BaseTextParser
{
private:

	bool currentListEmpty = false;

	bool readComponentNumber() override;
	void readTablePartData();

	void writeValueOfTwoSubStr(bool(DrawingPage::* trySetFunction)(const std::wstring&, bool), std::initializer_list<std::wstring> ifEqualStr, const std::wstring& firstStr = L"");
	void reset() override;
	bool readComponent() override;
public:
	TextParserNAG(const std::wstring& text, wchar_t separator);
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
};

