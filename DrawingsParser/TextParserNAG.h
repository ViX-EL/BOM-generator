#pragma once
#include "BaseTextParser.h"
#include <initializer_list>
#include <string>

class TextParserNAG : public BaseTextParser
{
private:

	bool currentListEmpty = false;

	bool readLastComponentNumber() override;
	void readTablePartData();

	void writeValueOfTwoSubStr(const std::wstring& columnName, std::initializer_list<std::wstring> ifEqualStr, const std::wstring& firstStr = L"");
public:
	TextParserNAG(const std::wstring& text, Columns& columns, std::vector<int>& componentsCountPerList, wchar_t separator);
	void parse(const std::wstring& fileName) override;
};

