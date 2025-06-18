#pragma once

#include "TextLoader.h"
#include "IMessagePrinter.h"

class DWGDotNetTextLoader : public TextLoader
{
public:
	explicit DWGDotNetTextLoader(IMessagePrinter* errorPrinter);
	int loadFile(const std::string& filePath) override;
};

