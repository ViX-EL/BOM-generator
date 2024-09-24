#include "TextLoader.h"

TextLoader::TextLoader(IMessagePrinter* printer)
{
	this->printer.reset(printer);
}

void TextLoader::printText(const std::wstring& label)
{
	printer->printText(text, label);
}

wchar_t TextLoader::getSeparator() const
{
	return separator;
}

void TextLoader::setSeparator(wchar_t separator)
{
	this->separator = separator;
}

const std::wstring& TextLoader::getText() const
{
	return text;
}

const std::wstring& TextLoader::getFileName() const
{
	return filename;
}