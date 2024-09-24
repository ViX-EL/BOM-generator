#include "TextParserPTE.h"

#include <regex>

bool TextParserPTE::readLastComponentNumber()
{
	std::wstring componentNumberStr(getNextSubString());
	//Если не найден номер компонента
	std::wregex componentNumberPattern(LR"( *\d+)");
	std::wregex lineNumberPattern(LR"(\d+-\w+-\d+-\w+-\d+)");
	bool isLineNumber = std::regex_match(componentNumberStr, lineNumberPattern);
	bool isComponentNumber = std::regex_match(componentNumberStr, componentNumberPattern);
	size_t positionInText = currentPositionInText;
	bool isListNumberFurther = std::regex_match(getNextSubString(positionInText), componentNumberPattern);
	if (!isComponentNumber || isLineNumber)
	{
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") && (lastComponentNumber != 0) || isLineNumber) {
			componentsCountPerList->push_back(lastComponentNumber);
		}
		return false;
	}
	else
	{
		if (isListNumberFurther) {
			return false;
		}
		lastComponentNumber++;
		return true;
	}
}

bool TextParserPTE::readComponent(Columns* columns)
{
	if (!readLastComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 3; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}

	std::wregex diameterPattern(LR"(\d{1,4} x( \d+)?|\d{1,4})");
	std::wregex materialPattern(LR"([a-zA-Z0-9 ]+ \/( [а-яА-Я0-9 ]+)?)"); //TODO исправить регулярку
	std::wregex countPattern(LR"( {0,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)");
	bool cases[]{ false, false };

	int descriptionSubStrCount{ 1 };
	int materialStrLength{ 1 };
	while (!cases[0] && !cases[1])
	{
		cases[0] = std::regex_match(*(subStrBuffer.end() - 2), diameterPattern);
		cases[0] = cases[0] && std::regex_match(*(subStrBuffer.end() - 1), countPattern);

		if (!cases[0])
		{
			cases[1] = std::regex_match(*(subStrBuffer.end() - 1), countPattern);
			if (cases[1] && std::regex_match(*(subStrBuffer.end() - 3), materialPattern)) {
				materialStrLength++;
				cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 4), diameterPattern);
			}
			else
			{
				cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 2), materialPattern);
				cases[1] = cases[1] && std::regex_match(*(subStrBuffer.end() - 3), diameterPattern);
			}
		}

		if (!cases[0] && !cases[1]) {
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	if (cases[0])
	{
		if ((subStrBuffer.end() - 2)->ends_with(L"x")) {
			subStrBuffer.emplace_back(getNextSubString());
		}
	}

	if (cases[1]) {
		descriptionSubStrCount--;
	}
	std::wstring descriptionStr;
	for (int i = 0; i < descriptionSubStrCount; i++)
	{
		descriptionStr += subStrBuffer[i];
	}
	(*columns)[L"Описание компонента"].emplace_back(descriptionStr);

	if (cases[0])
	{
		if ((subStrBuffer.end() - 3)->ends_with(L"x")) {
			(*columns)[L"Условный диаметр"].emplace_back((*(subStrBuffer.end() - 3) + L' ' + (*(subStrBuffer.end() - 2))));
		}
		else {
			(*columns)[L"Условный диаметр"].emplace_back(*(subStrBuffer.end() - 2));
		}
	}
	else if (cases[1])
	{
		if ((subStrBuffer.end() - 4)->ends_with(L"x")) {
			(*columns)[L"Условный диаметр"].emplace_back((*(subStrBuffer.end() - 4) + L' ' + (*(subStrBuffer.end() - 3))));
		}
		else {
			if (materialStrLength > 1) {
				(*columns)[L"Условный диаметр"].emplace_back(*(subStrBuffer.end() - 4));
			}
			else {
				(*columns)[L"Условный диаметр"].emplace_back(*(subStrBuffer.end() - 3));
			}
		}
	}
	(*columns)[L"Кол-во"].emplace_back(*(subStrBuffer.end() - 1));
	(*columns)[L"Код позиции"].emplace_back(L"-");
	(*columns)[L"Документ"].emplace_back(L"-");
	return true;
}

void TextParserPTE::readTablePartData()
{
	moveToPreviouslySubString(L"Prep/Разраб.");
	(*columns)[L"Листов"].emplace_back(getPreviouslySubString());
	(*columns)[L"Лист"].emplace_back(getPreviouslySubString());
	(*columns)[L"Номер линии"].emplace_back(getPreviouslySubString());
	(*columns)[L"Номер схемы"].emplace_back(getSubString(L"Номер схемы").erase(0, 14));
	(*columns)[L"Изометрический чертеж"].emplace_back(getSubString(L"Изометрический чертеж").erase(0, 22));
	(*columns)[L"Шифр документа"].emplace_back(getNextSubString());
	moveOnCountSubStr(3);
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
}

TextParserPTE::TextParserPTE(const std::wstring& text, Columns& columns, std::vector<int>& componentsCountPerList, wchar_t separator) :
	BaseTextParser(text, columns, componentsCountPerList, separator)
{

}

void TextParserPTE::parse(const std::wstring& fileName)
{
	reset();

	if (text->find(L"ERECTION MATERIALS", currentPositionInText) != std::wstring::npos) {
		moveToNextSubString(L"КОЛ-ВО");
	}
	else {
		return;
	}

	while (true) //Чтение всех компонентов
	{
		if (!readComponent(columns))
		{
			if (!readComponent(columns)) {
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
