#pragma once
#include "BaseTextParser.h"
class TextParserPTE : public BaseTextParser
{
private:
	bool readLastComponentNumber() override;
	bool readComponent(Columns* columns) override;
	void readTablePartData();

public:
	TextParserPTE(const std::wstring& text, Columns& columns, std::vector<int>& componentsCountPerList, wchar_t separator);
	void parse(const std::wstring& fileName) override;
};

