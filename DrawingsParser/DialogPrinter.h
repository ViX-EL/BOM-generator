﻿#pragma once

#ifndef DialogPrinter_h__
#define DialogPrinter_h__

#include "IMessagePrinter.h"
#include "TextDialog.h"

class DialogPrinter : public IMessagePrinter
{
private:
	TextDialog* textDialog;

public:
	DialogPrinter();
	void printError(const std::wstring& message) const override;
	void printText(const std::wstring& message, const std::wstring& label) override;
};

#endif // DialogPrinter_h__

