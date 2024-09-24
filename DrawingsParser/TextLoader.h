#pragma once

#include "IMessagePrinter.h"

#include <string>
#include <memory>

class TextLoader
{
protected:
	std::unique_ptr<IMessagePrinter> printer;
	std::wstring text;
	wchar_t separator{ '^' };
	std::wstring filename;
public:
	TextLoader(IMessagePrinter* printer);
	virtual int loadFile(const std::string& filename) = 0;
	void printText(const std::wstring& label);
	wchar_t getSeparator() const;
	void setSeparator(wchar_t separator);
	const std::wstring& getText() const;
	const std::wstring& getFileName() const;
};

