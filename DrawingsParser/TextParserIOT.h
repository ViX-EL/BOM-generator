#pragma once

#include "BaseTextParser.h"

class TextParserIOT : public BaseTextParser
{
private:
	bool readComponent();
	bool readLastComponentNumber() override;
	void readTablePartData();

public:
	void parse(const std::wstring& fileName) override;
	TextParserIOT(const std::wstring& text, Columns& columns, std::vector<int>& componentsCountPerList,
		wchar_t separator);
};

