#include "TextParserMEN.h"

bool TextParserMEN::readLastComponentNumber()
{
	return false;
}

void TextParserMEN::parse(const std::wstring& fileName)
{

}

TextParserMEN::TextParserMEN(const std::wstring& text, Columns& columns,
	std::vector<int>& componentsCountPerList, wchar_t separator) : BaseTextParser(text, columns,
		componentsCountPerList, separator)
{

}


