#pragma once
#include "BaseTextParser.h"
class TextParserNAG : public BaseTextParser
{
private:
	bool readLastComponentNumber();
	void readTablePartData();

public:
	TextParserNAG(const std::wstring& text, Columns& columns,
		std::vector<int>& componentsCountPerList, wchar_t separator);
	void parse(const std::wstring& fileName) override;
};

