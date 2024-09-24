#pragma once
#include "BaseTextParser.h"

#include <string>

class TextParserMEN : public BaseTextParser
{
private:
	bool readLastComponentNumber() override;

public:
	void parse(const std::wstring& fileName) override;
	TextParserMEN(const std::wstring& text, Columns& columns, std::vector<int>& componentsCountPerList,
		wchar_t separator);
};

