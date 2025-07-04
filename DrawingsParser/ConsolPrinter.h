﻿#pragma once

#ifndef ConsolPrinter_h__
#define ConsolPrinter_h__

#include "IMessagePrinter.h"
#include <string>

class ConsolPrinter : public IMessagePrinter
{
public:
	ConsolPrinter();
	void printError(const std::wstring& message) const override;
	void printText(const std::wstring& text, const std::wstring& label) override;
};

#endif // ConsolPrinter_h__

