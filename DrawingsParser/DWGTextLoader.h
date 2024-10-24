#pragma once

#include "IMessagePrinter.h"
#include "TextLoader.h"

#include <string>
#include <memory>

class DWGTextLoader : public TextLoader
{
public:
	DWGTextLoader(IMessagePrinter* errorPrinter);
	int loadFile(const std::string& filePath) override;
};

