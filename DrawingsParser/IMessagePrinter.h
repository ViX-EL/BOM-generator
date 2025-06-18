#pragma once

#ifndef IMessagePrinter_h__
#define IMessagePrinter_h__

#include <string>

class IMessagePrinter
{
public:
	virtual void printError(const std::wstring& message) const = 0;
	virtual void printText(const std::wstring& message, const std::wstring& label) = 0;
};

#endif // IMessagePrinter_h__

