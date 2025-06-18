#pragma once

#ifndef TextParserOZN_h__
#define TextParserOZN_h__

#include "BaseTextParser.h"

class TextParserOZN : public BaseTextParser
{
private:
	bool readComponentNumber() override;
	bool readComponent() override;
	void readTablePartData();
	std::wstring getValueFromSubStrWith(const std::wstring& subStr);

public:
	TextParserOZN(const std::wstring& text, wchar_t separator);
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
};

#endif // TextParserOZN_h__

