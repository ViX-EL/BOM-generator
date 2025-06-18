#pragma once

#ifndef DWGTextLoader_h__
#define DWGTextLoader_h__

#include "IMessagePrinter.h"
#include "TextLoader.h"
#include "DWGDotNetTextLoader.h"

#include "dwg.h"
#include "dwg_api.h"
#include <string>
#include <memory>

class DWGTextLoader : public TextLoader
{
private:
	const char* process_text(Dwg_Object* obj);
public:
	explicit DWGTextLoader(IMessagePrinter* errorPrinter);
	int loadFile(const std::string& filePath) override;
};

#endif // DWGTextLoader_h__

