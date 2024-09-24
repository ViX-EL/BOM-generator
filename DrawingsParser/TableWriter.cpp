#include "TableWriter.h"

#include "xlsxwriter.h"

#include "StringConvert.h"

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

void TableWriter::writeListsLines(uint16_t startRow) const
{
	size_t currentList{ 0 };
	uint16_t currentRow{ startRow };
	for (size_t i = 0; i < componentsCountPerList->size(); i++)
	{
		for (size_t j = 0; j < componentsCountPerList->at(currentList); j++)
		{
			const size_t componentsFieldsCount{ 5 };
			for (uint16_t compFieldIdx = 0; compFieldIdx <
				componentsFieldsCount; compFieldIdx++)
			{
				worksheet_write_string(worksheet, currentRow + 1,
					compFieldIdx, utf8_encode(
					columns->at(columnsNames[compFieldIdx])
					.at(currentRow)).c_str(), NULL);
			}

			for (uint16_t colIdx = componentsFieldsCount; colIdx <
				columnsNames.size(); colIdx++)
			{
				worksheet_write_string(worksheet, currentRow + 1, 
					colIdx, utf8_encode(columns->at(columnsNames[colIdx])
					.at(currentList)).c_str(), NULL);
			}
			currentRow++;
		}
		currentList++;
	}
}

void TableWriter::changeFileNameIfAlreadyExists(const std::string&
	tableDirectoryName)
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
			currFileName = tableFileName + " (" 
				+ std::to_string(alreadyExistsCount) + ')' + ".xlsx";
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

TableWriter::TableWriter(const BaseTextParser::Columns& columns, const 
	std::vector<int>& componentsCountPerList) :
	columns(&columns), componentsCountPerList(&componentsCountPerList),
	workbook(nullptr), worksheet(nullptr)
{

}

void TableWriter::writeTable() const
{
	writeHeders();

	writeListsLines(0);

	workbook_close(workbook);
}

void TableWriter::createNewTableFile(const std::string& tableDirectoryName)
{
	changeFileNameIfAlreadyExists(tableDirectoryName);

	if (tableDirectoryName != ".\\") {
		workbook = workbook_new((tableDirectoryName + "\\" 
			+ tableFileName).c_str());
	}
	else {
		workbook = workbook_new(tableFileName.c_str());
	}
	worksheet = workbook_add_worksheet(workbook, NULL);
}
