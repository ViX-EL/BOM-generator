#pragma once

#ifndef TEXTPARSERASP_H
#define TEXTPARSERASP_H

#include "BaseTextParser.h"

#include <string>

class TextParserASP : public BaseTextParser
{
private:
	bool readLastComponentNumber() override;
	bool readListAndListsCount(const std::wstring& listCountStr, const std::wstring& currentListStr);
	void readTablePartData();
	void parseCase1(std::wstring designTemperatureStr);
	void parseCase2();

public:
	TextParserASP(const std::wstring& text, Columns& columns,
		std::vector<int>& componentsCountPerList, wchar_t separator);
	void parse(const std::wstring& fileName) override;
};

#endif// TEXTPARSERASP_H

