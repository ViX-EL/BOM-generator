#pragma once

#ifndef TEXTPARSERLGN_H
#define TEXTPARSERLGN_H

#include "BaseTextParser.h"
#include "DrawingPageLGN.h"

#include <string>

class TextParserLGN : public BaseTextParser
{
public:
	TextParserLGN(const std::wstring& text, wchar_t separator);
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;

private:

	bool readComponent();
	bool readComponentNumber() override;
	bool readDescriptionAndDiameter(const std::wstring& descriptionStr, std::wstring& diameterStr);
	void readDocument();
	
	bool findCountStr();
	bool readList();
	bool isHaveOnlyOneList();
	void reset() override;

	bool lineBreak = false;
};

#endif // TEXTPARSERLGN_H

