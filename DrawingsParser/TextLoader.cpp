#include "TextLoader.h"
#include "StringConvert.h"

void TextLoader::setFileName(const std::wstring& filePath)
{
	size_t fileNameStartIdx = filePath.find_last_of(L'\\');
	fileName = filePath;
	if (fileNameStartIdx != std::wstring::npos) {
		fileName = filePath.substr(fileNameStartIdx + 1);
	}
}

void TextLoader::setFilePath(const std::string& filePath)
{
	this->filePath = utf8_decode(filePath);
}

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

const std::wstring& TextLoader::getFilePath() const
{
	return filePath;
}

const std::wstring& TextLoader::getFileName() const
{
	return fileName;
}
