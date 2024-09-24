#pragma once

#include "IMessagePrinter.h"

class DialogPrinter : public IMessagePrinter
{
public:
	void printError(const std::wstring& message) const override;
	void printText(const std::wstring& message, const std::wstring& label) override;
};

