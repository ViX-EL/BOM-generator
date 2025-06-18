#pragma once

#ifndef TextParserHMS_h__
#define TextParserHMS_h__

#include "BaseTextParser.h"

class TextParserHMS : public BaseTextParser
{
private:
	bool readComponentNumber() override;
	bool readComponent() override;
	void readTablePartData();
	bool isLineNumber(const std::wstring& sourceStr) const;

public:
	TextParserHMS(const std::wstring& text, wchar_t separator);
	void parse(const std::wstring& fileName, std::vector<Drawing>& drawings) override;
};

#endif // TextParserHMS_h__

