#pragma once

#ifndef TextParserFGG_h__
#define TextParserFGG_h__

#include "BaseTextParser.h"

class TextParserFGG : public BaseTextParser
{
private:
	bool readComponent();
	bool readComponentNumber() override;
	void readTablePartData();

	std::wstring continueListName;

public:
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
	TextParserFGG(const std::wstring& text, wchar_t separator);
};

#endif // TextParserFGG_h__
