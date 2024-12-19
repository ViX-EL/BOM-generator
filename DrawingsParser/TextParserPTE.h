#pragma once
#include "BaseTextParser.h"
class TextParserPTE : public BaseTextParser
{
private:
	bool readComponentNumber() override;
	bool readComponent() override;
	void readTablePartData();

public:
	TextParserPTE(const std::wstring& text, wchar_t separator);
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
};

