#pragma once

#ifndef TextLoader_h__
#define TextLoader_h__

#include "IMessagePrinter.h"

#include <string>
#include <memory>

class TextLoader
{
protected:
	std::unique_ptr<IMessagePrinter> printer;
	std::wstring text = L"";
	wchar_t separator{ '^' };
	std::wstring filePath;
	std::wstring fileName;

	void setFileName(const std::wstring& filePath);
	void setFilePath(const std::string& filePath);
public:
	explicit TextLoader(IMessagePrinter* printer);
	virtual int loadFile(const std::string& filePath) = 0;
	void printText(const std::wstring& label); //отладочная функция
	wchar_t getSeparator() const;
	void setSeparator(wchar_t separator);
	const std::wstring& getText() const;
	const std::wstring& getFilePath() const;
	const std::wstring& getFileName() const;
};

#endif // TextLoader_h__

