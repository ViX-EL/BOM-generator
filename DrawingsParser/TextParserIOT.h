#pragma once

#include "BaseTextParser.h"

class TextParserIOT : public BaseTextParser
{
private:
	int componentsCount = 0;
	bool readComponent();
	bool readComponentNumber() override;
	void readTablePartData();

	void reset() override;

public:
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
	TextParserIOT(const std::wstring& text,wchar_t separator);
};

