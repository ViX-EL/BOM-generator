#pragma once
#include "BaseTextParser.h"

#include <string>

class TextParserNAG : public BaseTextParser
{
private:
	bool readLastComponentNumber() override;
	void readTablePartData();

	void writeValueOfTwoSubStr(const std::wstring& columnName, const std::wstring& ifEqualStr);
public:
	TextParserNAG(const std::wstring& text, Columns& columns,
		std::vector<int>& componentsCountPerList, wchar_t separator);
	void parse(const std::wstring& fileName) override;
};

