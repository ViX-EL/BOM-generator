#pragma once

#ifndef TEXTPARSERASP_H
#define TEXTPARSERASP_H

#include "BaseTextParser.h"

#include <string>

class TextParserASP : public BaseTextParser
{
private:
	bool withoutWeldedPipe = false;
	bool existComponentsInEndText = false;
	bool listStartWithContFrom = false;
	std::wstring cipherDocumentStr;

	void writeComponentsCount();
	void readComponensFromEndText();
	bool readLastComponentNumber() override;
	bool readListAndListsCount(const std::wstring& listCountStr, const std::wstring& currentListStr);
	void readTablePartData();

	void readDescriptionsFromEndText();
	bool readFirstPipeInEndText(std::wstring& currentSubStr);
	void parseCase1(std::wstring designTemperatureStr);
	void parseCase2();
	void parseCase3();
	void setDashIfValueMissing(const std::wstring& nextSubStr, const std::wstring& unwantedSubStr, const std::wstring& columnName);

public:
	TextParserASP(const std::wstring& text, Columns& columns, std::vector<int>& componentsCountPerList, wchar_t separator);
	void parse(const std::wstring& fileName) override;
	void reset() override;
};

#endif// TEXTPARSERASP_H

