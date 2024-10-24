#include "PDFTextLoader.h"
#include "StringConvert.h"

#include "podofo/podofo.h"

#include <vector>
#include <memory>

bool PDFTextLoader::isIncludedInBoundings(const std::pair<double, double>& point) const
{
	if (boundings.leftTopCorner < point && boundings.rightBottomCorner > point) {
		return true;
	}
	return false;
}

PDFTextLoader::PDFTextLoader(IMessagePrinter* printer) : TextLoader(printer)
{

}

int PDFTextLoader::loadFile(const std::string& filePath)
{
	using namespace PoDoFo;

	text.clear();
	setFilePath(filePath);
	setFileName(getFilePath());

	PdfMemDocument doc;
	doc.Load(filePath);

	auto& pages = doc.GetPages();

	for (unsigned int i = 0; i < pages.GetCount(); ++i) {
		PoDoFo::PdfPage* page = &pages.GetPageAt(i);
		std::vector<PdfTextEntry> textEntries;
		page->ExtractTextTo(textEntries);
		for (auto& textEntry : textEntries)
		{
			//if (isIncludedInBoundings({ textEntry.X, textEntry.Y })) {
				text.append(utf8_decode(textEntry.Text) + separator);
			//}
		}
	}
	return 1;
}

void PDFTextLoader::setBoundings(const Boundings& boundings)
{
	this->boundings = boundings;
}
