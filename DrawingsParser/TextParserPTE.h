#pragma once

#ifndef TextParserPTE_h__
#define TextParserPTE_h__

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

#endif // TextParserPTE_h__

