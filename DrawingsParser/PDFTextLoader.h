#pragma once

#include "IMessagePrinter.h"
#include "TextLoader.h"

#include <string>
#include <memory>
#include <utility>

struct Boundings
{
	std::pair<double, double> leftTopCorner;
	std::pair<double, double> rightBottomCorner;
};

class PDFTextLoader : public TextLoader
{
private:
	Boundings boundings;
	bool isIncludedInBoundings(const std::pair<double, double>& point) const;

public:

	PDFTextLoader(IMessagePrinter* printer);
	int loadFile(const std::string& filePath) override;
	void setBoundings(const Boundings& boundings);
};

