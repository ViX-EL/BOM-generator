#include "TextParserNAG.h"

#include <regex>

bool TextParserNAG::readLastComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	//Если не найден номер компонента
	std::wregex componentNumberPattern(LR"( *\d+)");
	std::wregex lineNumberPattern(LR"(\d+-\d+-\d+\/\d+-\w+-\d+-[0-9A-Z]+-\d+)");
	bool isLineNumber = std::regex_match(componentNumberStr, lineNumberPattern);
	if (!std::regex_match(componentNumberStr, componentNumberPattern) || isLineNumber)
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") ||
			componentNumberStr.starts_with(L"UT PIPE LENGTH")) {
			if (lastComponentNumber != 0) {
				componentsCountPerList->push_back(lastComponentNumber);
				moveToPreviouslySubString(L"Формат");
				moveToPreviouslySubString();
				(*columns)[L"Листов"].emplace_back(getPreviouslySubString());
				(*columns)[L"Лист"].emplace_back(getPreviouslySubString());
				(*columns)[L"Номер линии"].emplace_back(getPreviouslySubString());
				moveOnCountSubStr(2);
			}
		}
		else if (componentNumberStr.starts_with(L"ERECTION MATERIALS")) {
			moveToSubString(L"КОЛ-ВО");
		}
		else if (isLineNumber) {
			(*columns)[L"Номер линии"].emplace_back(componentNumberStr);
			(*columns)[L"Лист"].emplace_back(getNextSubString());
			(*columns)[L"Листов"].emplace_back(getNextSubString());
			componentsCountPerList->push_back(lastComponentNumber);
		}
		return false;
	}
	else
	{
		lastComponentNumber++;
		return true;
	}
}

void TextParserNAG::readTablePartData()
{
	(*columns)[L"Номер схемы"].emplace_back(getNextSubString(L"ЛИСТОВ"));
	(*columns)[L"Расчет. Давление"].emplace_back(getNextSubString());
	(*columns)[L"Расчет. Темп"].emplace_back(getNextSubString());
	(*columns)[L"Рабочее давление"].emplace_back(getNextSubString());
	(*columns)[L"Рабочая температура"].emplace_back(getNextSubString());
	(*columns)[L"Давление испыт"].emplace_back(getNextSubString());
	(*columns)[L"Среда испытаний"].emplace_back(getNextSubString());
	(*columns)[L"Контроль сварных швов"].emplace_back(getNextSubString());
	(*columns)[L"Послесвар. Термообраб"].emplace_back(getNextSubString());
	(*columns)[L"Расчет напряжений"].emplace_back(getNextSubString());
	(*columns)[L"Система покраски"].emplace_back(getNextSubString());
	(*columns)[L"Спутниковый обогрев"].emplace_back(getNextSubString());
	(*columns)[L"Изоляция"].emplace_back(getNextSubString());
	(*columns)[L"Технологическая среда"].emplace_back(getNextSubString());
	(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getNextSubString());
	(*columns)[L"Категория трубопр. Гост"].emplace_back(getNextSubString());
	(*columns)[L"Диаметр трубопровода"].emplace_back(getNextSubString());
	(*columns)[L"Класс трубопровода"].emplace_back(getNextSubString());
	(*columns)[L"Шифр документа"].emplace_back(getNextSubString());
	(*columns)[L"Изометрический чертеж"].emplace_back(getNextSubString(L"Isometric drawing"));
}

TextParserNAG::TextParserNAG(const std::wstring& text, Columns& columns,
std::vector<int>& componentsCountPerList, wchar_t separator) : BaseTextParser(text, columns,
	componentsCountPerList, separator)
{

}

void TextParserNAG::parse(const std::wstring& fileName)
{
	reset();

	if (text->find(L"FABRICATION MATERIALS", currentPositionInText) != std::wstring::npos) {
		moveToSubString(L"КОЛ-ВО");
	}
	while (true) //Чтение всех компонентов
	{
		if (!BaseTextParser::readComponent(columns))
		{
			if (!BaseTextParser::readComponent(columns)) {
				break;
			}
		}
	}

	if (!componentsCountPerList->empty())
	{
		readTablePartData();

		(*columns)[L"Имя файла"].emplace_back(fileName);
	}
}
