#pragma once

#include "IMessagePrinter.h"
#include <string>

class WXLogPrinter : public IMessagePrinter
{
	void printError(const std::wstring& message) const override;
	void printText(const std::wstring& message, const std::wstring& label) override;
};

