#include "StringConvert.h"

#include "DOCXPrinter.h"
#include <duckx.hpp>
#include <string>

void DOCXPrinter::printError(const std::wstring& message) const
{

}

void DOCXPrinter::printText(const std::wstring& message, const std::wstring& fileName)
{
	duckx::Document doc(utf8_encode(fileName));
	duckx::Paragraph paragraph = doc.paragraphs();

	paragraph.add_run(utf8_encode(message));

	doc.save();
}
