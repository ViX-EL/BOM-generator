#pragma once

#ifndef TextParserSEC_h__
#define TextParserSEC_h__

#include "BaseTextParser.h"
#include "DrawingPage.h"
#include "ValuesCheker.h"

#include <initializer_list>
#include <string>
#include <vector>

class TextParserSEC : public BaseTextParser
{
private:
	bool readComponentNumber() override;
	bool readComponent() override;
	void readTablePartData();
	bool isEndOfComponent(const std::wstring& stringAfterComponent) const;

public:
	TextParserSEC(const std::wstring& text, wchar_t separator);
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
};

#endif // TextParserSEC_h__

