#pragma once

#ifndef TEXTPARSERLGN_H
#define TEXTPARSERLGN_H

#include "BaseTextParser.h"

#include <string>

class TextParserLGN : public BaseTextParser
{
public:
	TextParserLGN(const std::wstring& text, Columns& columns,
		std::vector<int>& componentsCountPerList, wchar_t separator);
	void parse(const std::wstring& fileName) override;

private:
	bool readComponent();
	bool readLastComponentNumber() override;
	bool readDescriptionAndDiameter(const std::wstring& descriptionStr,
		std::wstring& diameterStr);
	void readDocument();
	bool readList();
	bool isHaveOnlyOneList();
};

#endif // TEXTPARSERLGN_H

