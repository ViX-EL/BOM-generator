#pragma once

#include <string>

class IMessagePrinter
{
public:
	virtual void printError(const std::wstring& message) const = 0;
	virtual void printText(const std::wstring& message, const std::wstring& label) = 0;
};

