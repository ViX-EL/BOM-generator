#pragma once

#include "BaseTextParser.h"

#include "xlsxwriter.h"

#include <string>
#include <vector>

class TableWriter
{
private:
	const BaseTextParser::Columns* columns;
	const std::vector<std::wstring> columnsNames { 
		L"Описание компонента", L"Условный диаметр", L"Документ", 
		L"Кол-во", L"Код позиции", L"Листов", L"Лист", L"Рабочая температура",
		L"Рабочее давление", L"Спутниковый обогрев", L"Класс трубопровода",
		L"Технологическая среда", L"Среда испытаний", L"Система покраски",
		L"Послесвар. Термообраб", L"Контроль сварных швов", L"Давление испыт",
		L"Категория трубопр. Гост", L"Расчет. Темп", L"Расчет. Давление",
		L"Шифр документа", L"Диаметр трубопровода", L"Изоляция",
		L"Категория трубопр. ТР ТС", L"Номер схемы", L"Номер линии",
		L"Расчет напряжений", L"Изометрический чертеж", L"Имя файла"
	};
	const std::vector<int>* componentsCountPerList;
	lxw_workbook* workbook{ nullptr };
	lxw_worksheet* worksheet{nullptr};

	std::string tableFileName{"IsoBOM"};

	void writeHeders() const;
	void writeListsLines(uint16_t startRow) const;
	void changeFileNameIfAlreadyExists(const std::string& tableDirectoryName);

public:
	TableWriter(const BaseTextParser::Columns& columns, const std::vector<int>&
		componentsCountPerList);
	void writeTable() const;
	void createNewTableFile(const std::string& tableDirectoryName);
	const std::string& getFileName();
};

