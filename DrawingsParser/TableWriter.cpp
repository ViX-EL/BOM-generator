#include "TableWriter.h"

#include "xlsxwriter.h"

#include "StringConvert.h"
#include "IMessagePrinter.h"

#include <wx/log.h>
#include <boost/log/trivial.hpp>
#include <string>
#include <vector>
#include <filesystem>

void TableWriter::writeHeders() const
{
	for (uint16_t i = 0; i < columnsNames.size(); i++)
	{
		worksheet_write_string(worksheet, 0, i, utf8_encode(columnsNames[i]).c_str(), NULL);
	}
}

void TableWriter::writePagesFields()
{
	uint16_t currentRow{ 1 };
	uint16_t currentColumn{ 0 };

	for (size_t drawingIdx = 0; drawingIdx < drawings->size(); drawingIdx++)
	{
		for (size_t pageIdx = 0; pageIdx < (*drawings)[drawingIdx].pages.size(); pageIdx++)
		{
			std::shared_ptr<DrawingPage> currentDrawingPage = (*drawings)[drawingIdx].pages[pageIdx];
			for (size_t componentIdx = 0; componentIdx < (*drawings)[drawingIdx].pages[pageIdx]->getComponentsCount(); componentIdx++)
			{
				BuildComponent* currentComponentPtr = &currentDrawingPage->getComponent(componentIdx);

				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getElementName());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getType1());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getType2());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getType3());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getProductStandard());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getProductStandardENG());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getSteelGrade());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getSteelGradeENG());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getDiameter1());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getDiameter2());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getWallThickness1());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getWallThickness2());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getNominalDiameter1());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getNominalDiameter2());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getPressureNominal());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getPressureClass());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getASMEThickness1());
				writeCell(currentRow, currentColumn, currentComponentPtr->getSplitData()->getASMEThickness2());

				currentFileName = currentDrawingPage->getFileName();
				writeCell(currentRow, currentColumn, currentDrawingPage->getCipherDocument());
				writeCell(currentRow, currentColumn, currentDrawingPage->getLineNumber());
				writeCell(currentRow, currentColumn, currentDrawingPage->getIsometricDrawing());
				writeCell(currentRow, currentColumn, currentDrawingPage->getFileName());
				writeCell(currentRow, currentColumn, currentDrawingPage->getTotalPages());
				writeCell(currentRow, currentColumn, currentDrawingPage->getCurrentPage());

				writeCell(currentRow, currentColumn, currentComponentPtr->getPositionNumber());
				writeCell(currentRow, currentColumn, currentComponentPtr->getDescription());
				writeCell(currentRow, currentColumn, currentComponentPtr->getNominalDiameter());
				writeCell(currentRow, currentColumn, currentComponentPtr->getDocument());
				writeCell(currentRow, currentColumn, currentComponentPtr->getAmount());
				writeCell(currentRow, currentColumn, currentComponentPtr->getPositionCode());

				writeCell(currentRow, currentColumn, currentDrawingPage->getDiameterPipeline());
				writeCell(currentRow, currentColumn, currentDrawingPage->getOperatingTemperature());
				writeCell(currentRow, currentColumn, currentDrawingPage->getOperatingPressure());
				writeCell(currentRow, currentColumn, currentDrawingPage->getTracing());
				writeCell(currentRow, currentColumn, currentDrawingPage->getPipelineClass());
				writeCell(currentRow, currentColumn, currentDrawingPage->getTechnologicalEnvironment());
				writeCell(currentRow, currentColumn, currentDrawingPage->getTestEnvironment());
				writeCell(currentRow, currentColumn, currentDrawingPage->getPaintingSystem());
				writeCell(currentRow, currentColumn, currentDrawingPage->getPostWeldingHeatTreatment());
				writeCell(currentRow, currentColumn, currentDrawingPage->getWeldInspection());
				writeCell(currentRow, currentColumn, currentDrawingPage->getTestPressure());
				writeCell(currentRow, currentColumn, currentDrawingPage->getGOSTPipelineCategory());
				writeCell(currentRow, currentColumn, currentDrawingPage->getDesignTemperature());
				writeCell(currentRow, currentColumn, currentDrawingPage->getDesignPressure());
				writeCell(currentRow, currentColumn, currentDrawingPage->getIsolation());
				writeCell(currentRow, currentColumn, currentDrawingPage->getCategoryPipelinesTRCU());
				writeCell(currentRow, currentColumn, currentDrawingPage->getSchemeNumber());
				writeCell(currentRow, currentColumn, currentDrawingPage->getStressCalculation());
			}

			wxLogMessage("[Запись] Конец записи листа %d файла %s", currentDrawingPage->getCurrentPage(), currentDrawingPage->getFileName());
		}
	}
}

void TableWriter::writeCell(uint16_t& row, uint16_t& column, const std::wstring& cellValueStr) const
{
	worksheet_write_string(worksheet, row, column, utf8_encode(cellValueStr).c_str(), NULL);
	incrementCell(row, column);
}

void TableWriter::writeCell(uint16_t& row, uint16_t& column, int cellValue) const
{
	if (cellValue == 0) {
		worksheet_write_string(worksheet, row, column, "-", NULL);
	}
	else {
		worksheet_write_number(worksheet, row, column, cellValue, NULL);
	}
	incrementCell(row, column);
}

void TableWriter::writeCell(uint16_t& row, uint16_t& column, double cellValue) const
{
	if (cellValue == 0.0) {
		worksheet_write_string(worksheet, row, column, "-", NULL);
	}
	else {
		worksheet_write_number(worksheet, row, column, cellValue, NULL);
	}
	incrementCell(row, column);
}

void TableWriter::incrementCell(uint16_t& row, uint16_t& column) const
{
	column++;
	if (column == columnsNames.size()) {
		row++;
		column = 0;
	}
}

void TableWriter::changeFileNameIfAlreadyExists(const std::wstring& tableDirectoryName)
{
	namespace fs = std::filesystem;

	try
	{
		int alreadyExistsCount{ 0 };
		for (auto& dirIter : fs::directory_iterator(tableDirectoryName))
		{
			if (!fs::is_regular_file(dirIter.status())) {
				continue;
			}

			std::wstring currfileName = dirIter.path().filename().wstring();
			if (currfileName.compare(0, tableFileName.size(), tableFileName) == 0) {
				alreadyExistsCount++;
			}
		}

		if (alreadyExistsCount > 0) {
			std::wstring currFileName;
			do {
				currFileName = tableFileName + " (" + std::to_string(alreadyExistsCount) + ')' + ".xlsx";
				alreadyExistsCount++;
			} while (fs::exists(tableDirectoryName + L'\\' + currFileName));
			tableFileName = currFileName;
		}
		else {
			tableFileName += L".xlsx";
		}
	}
	catch (const std::exception& ex)
	{
		printer->printError(L"Ошибка создания файла таблицы: " + utf8_decode(ex.what()));
	}
}

const std::wstring& TableWriter::getFileName() const
{
	return tableFileName;
}

TableWriter::TableWriter(const std::vector<Drawing>& drawings, IMessagePrinter* printer) : drawings(&drawings), workbook(nullptr), worksheet(nullptr), printer(printer)
{

}

void TableWriter::writeTable()
{
	try {
		writeHeders();

		writePagesFields();

		workbook_close(workbook);
	}
	catch (const std::exception& ex) {
		printer->printError(L"Ошибка заполнения файла таблицы: " + utf8_decode(ex.what()) + L" \nВ файле " + currentFileName);
	}
}

void TableWriter::createNewTableFile(const std::wstring& tableDirectoryName)
{
	changeFileNameIfAlreadyExists(tableDirectoryName);

	if (tableDirectoryName != ".\\") {
		workbook = workbook_new((utf8_encode(tableDirectoryName + L'\\' + tableFileName)).c_str());
	}
	else {
		workbook = workbook_new(utf8_encode(tableFileName).c_str());
	}
	worksheet = workbook_add_worksheet(workbook, NULL);
}
