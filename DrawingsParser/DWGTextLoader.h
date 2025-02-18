#pragma once

#include "IMessagePrinter.h"
#include "TextLoader.h"

#include <string>
#include <memory>

class DWGTextLoader : public TextLoader
{
public:
	explicit DWGTextLoader(IMessagePrinter* errorPrinter);
	int loadFile(const std::string& filePath) override;
};

