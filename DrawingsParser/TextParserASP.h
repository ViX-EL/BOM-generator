#pragma once

#ifndef TEXTPARSERASP_H
#define TEXTPARSERASP_H

#include "BaseTextParser.h"
#include "DrawingPageASP.h"
#include "ValuesCheker.h"

#include <string>
#include <memory>

class TextParserASP : public BaseTextParser
{
private:
	bool withoutWeldedPipe = false;
	bool existComponentsInEndText = false;
	bool pageStartWithContFrom = false;

	std::wstring cipherDocumentStr;
	std::wstring designer;

	void readComponensFromEndText();
	bool readComponentNumber() override;
	bool readComponent() override;
	void readTablePartData();

	void readDescriptionsFromEndText();
	bool readFirstPipeInEndText(std::wstring& currentSubStr);
	void parseCase1(std::wstring designTemperatureStr);
	void parseCase2();
	void parseCase3();
	void trySet(const std::wstring& nextSubStr, const std::wstring& unwantedSubStr, bool(DrawingPage::* trySetFunction)(const std::wstring&, ValuesCheker::Type),
		ValuesCheker::Type chekType = ValuesCheker::getType());

public:
	TextParserASP(const std::wstring& text, wchar_t separator);
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
	void reset() override;
};

#endif// TEXTPARSERASP_H

