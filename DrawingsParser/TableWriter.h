#pragma once

#include "BaseTextParser.h"

#include "xlsxwriter.h"
#include "IMessagePrinter.h"

#include <string>
#include <vector>
#include <memory>
#include <string_view>

class TableWriter
{
private:
	size_t currentList = 0;
	std::unique_ptr<IMessagePrinter> printer;
	const std::vector<Drawing>* drawings;
	const std::vector<std::wstring> columnsNames { 
		L"Наименование элемента", L"Тип 1", L"Тип 2", L"Тип 3", L"Стандарт изделия", L"Стандарт изделия ENG", L"Марка стали", L"Марка стали ENG", L"Диаметр 1", L"Диаметр 2", 
		L"Толщина стенки 1", L"Толщина стенки 2", L"Условный диаметр 1", L"Условный диаметр 2", L"Условное давление", L"Класс давления", L"ASME толщина 1", L"ASME толщина 2",
		L"Шифр документа", L"Номер линии", L"Изометрический чертеж", L"Имя файла", L"Листов", L"Лист", L"Номер позиции", L"Описание компонента", L"Условный диаметр", L"Документ",
		L"Кол - во", L"Код позиции", L"Диаметр трубопровода", L"Рабочая температура", L"Рабочее давление", L"Спутниковый обогрев", L"Класс трубопровода", L"Технологическая среда",
		L"Среда испытаний", L"Система покраски", L"Послесварочная Термообработка", L"Контроль сварных швов", L"Давление испытаний", L"Категория трубопровода Гост",
		L"Расчетная Температура", L"Расчетное Давление", L"Изоляция", L"Категория трубопровода ТР ТС", L"Номер схемы", L"Расчет напряжений"
	};
	lxw_workbook* workbook{ nullptr };
	lxw_worksheet* worksheet{nullptr};

	std::wstring tableFileName{L"IsoBOM"};
	std::wstring currentFileName;

	void writeHeders() const;
	void writePagesFields();
	void writeCell(uint16_t& row, uint16_t& column, std::wstring_view cellValueStr) const;
	void writeCell(uint16_t& row, uint16_t& column, const std::wstring& cellValueStr) const;
	void writeCell(uint16_t& row, uint16_t& column, int cellValue) const;
	void writeCell(uint16_t& row, uint16_t& column, double cellValue) const;
	void incrementCell(uint16_t& row, uint16_t& column) const;
	void changeFileNameIfAlreadyExists(const std::wstring& tableDirectoryName);

public:
	TableWriter(const std::vector<Drawing>& drawings, IMessagePrinter* printer);
	void writeTable();
	void createNewTableFile(const std::wstring& tableDirectoryName);
	const std::wstring& getFileName() const;
};

