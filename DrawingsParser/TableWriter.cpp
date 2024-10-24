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
		worksheet_write_string(worksheet, 0, i,
			utf8_encode(columnsNames[i]).c_str(), NULL);
	}
}

void TableWriter::writeListsLines(uint16_t startRow)
{
	currentList = 0;
	uint16_t currentRow{ startRow };

	for (size_t i = 0; i < componentsCountPerList->size(); i++) //������ ������ �� ���� ������ ���������������
	{
		wxString currentFileName(columns->at(L"��� �����").at(currentList));
		wxString currentListNumber(columns->at(L"����").at(currentList));
		wxLogMessage("[������] ������ ������ ����� %s ����� %s", currentListNumber, currentFileName);
		//BOOST_LOG_TRIVIAL(trace) << "������ ������ ������ ����� " << utf8_encode(currentFileName);

		for (size_t j = 0; j < componentsCountPerList->at(currentList); j++) //������ ����� �������� �����
		{
			const size_t componentsFieldsCount{ 5 };
			for (uint16_t compFieldIdx = 0; compFieldIdx < componentsFieldsCount; compFieldIdx++)
			{
				std::wstring currentCellValue = columns->at(columnsNames[compFieldIdx]).at(currentRow);
				worksheet_write_string(worksheet, currentRow + 1, compFieldIdx, utf8_encode(currentCellValue).c_str(), NULL);
			}

			for (uint16_t colIdx = componentsFieldsCount; colIdx < columnsNames.size(); colIdx++)
			{
				std::wstring currentCellValue = columns->at(columnsNames[colIdx]).at(currentList);
				worksheet_write_string(worksheet, currentRow + 1, colIdx, utf8_encode(currentCellValue).c_str(), NULL);
			}

			currentRow++;
		}

		//BOOST_LOG_TRIVIAL(trace) << "������� ���� " << utf8_encode(*lastListNumberIter) << " ����� " << utf8_encode(currentFileName);
		wxLogMessage("[������] ����� ������ ����� %s ����� %s", currentListNumber, currentFileName);

		currentList++;
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

const std::string& TableWriter::getFileName()
{
	return tableFileName;
}

TableWriter::TableWriter(const BaseTextParser::Columns& columns, const  std::vector<int>& componentsCountPerList, IMessagePrinter* printer) :
	columns(&columns), componentsCountPerList(&componentsCountPerList), workbook(nullptr), worksheet(nullptr), printer(printer)
{

}

void TableWriter::writeTable()
{
	try {
		writeHeders();

		writeListsLines(0);

		workbook_close(workbook);
	}
	catch (const std::exception& ex) {
		printer->printError(L"������ ���������� ����� �������: " + utf8_decode(ex.what()) + L" \n� ����� " 
			+ columns->at(L"��� �����").at(currentList));
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
