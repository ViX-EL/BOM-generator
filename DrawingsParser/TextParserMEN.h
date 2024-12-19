#pragma once
#include "BaseTextParser.h"

#include <string>

class TextParserMEN : public BaseTextParser
{
private:
	bool readComponentNumber() override;

public:
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
	TextParserMEN(const std::wstring& text, wchar_t separator);
};

