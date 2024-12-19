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
			for (size_t componentIdx = 0; componentIdx < (*drawings)[drawingIdx].pages[pageIdx]->getComponentsCount(); componentIdx++)
			{
				currentFileName = (*drawings)[drawingIdx].pages[pageIdx]->getFileName();
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getCipherDocument());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getLineNumber());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getIsometricDrawing());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getFileName());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getTotalPages());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getCurrentPage());

				BuildComponent* currentComponentPtr = &(*drawings)[drawingIdx].pages[pageIdx]->getComponent(componentIdx);
				writeCell(currentRow, currentColumn, currentComponentPtr->getPositionNumber());
				writeCell(currentRow, currentColumn, currentComponentPtr->getDescription());
				writeCell(currentRow, currentColumn, currentComponentPtr->getNominalDiameter());
				writeCell(currentRow, currentColumn, currentComponentPtr->getDocument());
				writeCell(currentRow, currentColumn, currentComponentPtr->getAmount());
				writeCell(currentRow, currentColumn, currentComponentPtr->getPositionCode());

				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getDiameterPipeline());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getOperatingTemperature());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getOperatingPressure());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getTracing());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getPipelineClass());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getTechnologicalEnvironment());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getTestEnvironment());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getPaintingSystem());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getPostWeldingHeatTreatment());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getWeldInspection());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getTestPressure());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getGOSTPipelineCategory());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getDesignTemperature());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getDesignPressure());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getIsolation());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getCategoryPipelinesTRCU());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getSchemeNumber());
				writeCell(currentRow, currentColumn, (*drawings)[drawingIdx].pages[pageIdx]->getStressCalculation());
			}

			wxLogMessage("[Запись] Конец записи листа %d файла %s", (*drawings)[drawingIdx].pages[pageIdx]->getCurrentPage(), (*drawings)[drawingIdx].pages[pageIdx]->getFileName());
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
	worksheet_write_number(worksheet, row, column, cellValue, NULL);
	incrementCell(row, column);
}

void TableWriter::incrementCell(uint16_t& row, uint16_t& column) const
{
	column++;
	if (column == 30) {
		row++;
		column = 0;
	}
}

void TableWriter::changeFileNameIfAlreadyExists(const std::string& tableDirectoryName)
{
	namespace fs = std::filesystem;

	int alreadyExistsCount{ 0 };
	for (auto& dirIter : fs::directory_iterator(tableDirectoryName))
	{
		if (!fs::is_regular_file(dirIter.status())) {
			continue;
		}

		std::string currfileName = dirIter.path().filename().string();
		if (currfileName.compare(0, tableFileName.size(), tableFileName) == 0) {
			alreadyExistsCount++;
		}
	}

	if (alreadyExistsCount > 0) {
		std::string currFileName;
		do {
			currFileName = tableFileName + " (" + std::to_string(alreadyExistsCount) + ')' + ".xlsx";
			alreadyExistsCount++;
		} while (fs::exists(tableDirectoryName + '\\' + currFileName));
		tableFileName = currFileName;
	} 
	else {
		tableFileName += ".xlsx";
	}
}

const std::string& TableWriter::getFileName() const
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

void TableWriter::createNewTableFile(const std::string& tableDirectoryName)
{
	changeFileNameIfAlreadyExists(tableDirectoryName);

	if (tableDirectoryName != ".\\") {
		workbook = workbook_new((tableDirectoryName + "\\" + tableFileName).c_str());
	}
	else {
		workbook = workbook_new(tableFileName.c_str());
	}
	worksheet = workbook_add_worksheet(workbook, NULL);
}
