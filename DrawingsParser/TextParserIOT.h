#pragma once

#include "BaseTextParser.h"

class TextParserIOT : public BaseTextParser
{
private:
	bool readComponent();
	bool readComponentNumber() override;
	void readTablePartData();

public:
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
	TextParserIOT(const std::wstring& text,wchar_t separator);
};

