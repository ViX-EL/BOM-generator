#include "TextParserIOT.h"
#include "BaseTextParser.h"

#include <regex>
#include <string>

TextParserIOT::TextParserIOT(const std::wstring& text, Columns& columns, std::vector<int>& componentsCountPerList, wchar_t separator) : 
	BaseTextParser(text, columns, componentsCountPerList, separator)
{

}

bool TextParserIOT::readComponent()
{
	if (!readLastComponentNumber()) {
		return false;
	}

	std::vector<std::wstring> subStrBuffer;

	for (int i = 0; i < 4; i++) {
		subStrBuffer.emplace_back(getNextSubString());
	}

	std::wregex diameterPattern(LR"(\d{1,4} x \d{1,4}|\d{1,4})");
	std::wregex itemCodePattern(LR"([a-zA-Z0-9()-]+)");
	std::wregex countPattern(LR"( {0,2}\d{1,3}M?| {0,2}\d{1,3}\.\d{1,4}M)");
	bool match{ false };

	int descriptionSubStrCount{ 1 };
	while (!match)
	{
		match = std::regex_match(*(subStrBuffer.end() - 3), diameterPattern);
		match = match && std::regex_match(*(subStrBuffer.end() - 2), itemCodePattern);
		match = match && std::regex_match(*(subStrBuffer.end() - 1), countPattern);

		if (!match) {
			subStrBuffer.emplace_back(getNextSubString());
			descriptionSubStrCount++;
		}
	}

	std::wstring descriptionStr;
	for (int i = 0; i < descriptionSubStrCount; i++)
	{
		descriptionStr += subStrBuffer[i];
	}
	(*columns)[L"Описание компонента"].emplace_back(descriptionStr);

	(*columns)[L"Условный диаметр"].emplace_back(*(subStrBuffer.end() - 3));
	(*columns)[L"Код позиции"].emplace_back(*(subStrBuffer.end() - 2));
	(*columns)[L"Кол-во"].emplace_back(*(subStrBuffer.end() - 1));

	(*columns)[L"Документ"].emplace_back(L"-");
	return true;
}

bool TextParserIOT::readLastComponentNumber()
{
	if (componentsEnded) {
		return false;
	}

	std::wstring componentNumberStr(getNextSubString());
	//Если не найден номер компонента
	std::wregex componentNumberPattern(LR"( *\d+)");
	if (!std::regex_match(componentNumberStr, componentNumberPattern)) {
		if (componentNumberStr.starts_with(L"CUT PIPE LENGTH") || componentNumberStr.starts_with(L"GCC-IOT-DDD")) {
			componentsCountPerList->push_back(lastComponentNumber);
			componentsEnded = true;
		}
		return false;
	}
	else
	{
		lastComponentNumber++;
		return true;
	}
}

void TextParserIOT::readTablePartData()
{
	(*columns)[L"Класс трубопровода"].emplace_back(getPreviouslySubString(L"TOTAL FOR"));
	(*columns)[L"Диаметр трубопровода"].emplace_back(getPreviouslySubString());
	(*columns)[L"Изоляция"].emplace_back(getPreviouslySubString());
	(*columns)[L"Спутниковый обогрев"].emplace_back(getPreviouslySubString());
	(*columns)[L"Рабочая температура"].emplace_back(getPreviouslySubString());
	(*columns)[L"Расчет. Темп"].emplace_back(getPreviouslySubString());
	(*columns)[L"Рабочее давление"].emplace_back(getPreviouslySubString());
	(*columns)[L"Давление испыт"].emplace_back(getPreviouslySubString());
	(*columns)[L"Расчет. Давление"].emplace_back(getPreviouslySubString());
	moveToPreviouslySubString();
	(*columns)[L"Номер схемы"].emplace_back(getPreviouslySubString());
	(*columns)[L"Изометрический чертеж"].emplace_back(getSubString(L"Isometric drawing", true).erase(0, 18));
	moveToPreviouslySubString();
	(*columns)[L"Имя файла"].emplace_back(getPreviouslySubString());
	moveOnCountSubStr(8, true);
	(*columns)[L"Номер линии"].emplace_back(getPreviouslySubString());
	moveOnCountSubStr(3, true);
	(*columns)[L"Листов"].emplace_back(getPreviouslySubString());
	(*columns)[L"Лист"].emplace_back(getPreviouslySubString());
	moveToPreviouslySubString();
	(*columns)[L"Шифр документа"].emplace_back(getPreviouslySubString());
	(*columns)[L"Система покраски"].emplace_back(getNextSubString(L"ТЕХНОЛОГИИ"));
	(*columns)[L"Среда испытаний"].emplace_back(getNextSubString());
	(*columns)[L"Контроль сварных швов"].emplace_back(getNextSubString());
	(*columns)[L"Послесвар. Термообраб"].emplace_back(getNextSubString());
	(*columns)[L"Расчет напряжений"].emplace_back(getNextSubString());
	(*columns)[L"Технологическая среда"].emplace_back(getNextSubString());
	(*columns)[L"Категория трубопр. ТР ТС"].emplace_back(getNextSubString());
	(*columns)[L"Категория трубопр. Гост"].emplace_back(getNextSubString());
}

void TextParserIOT::parse(const std::wstring& fileName)
{
	reset();

	moveToNextSubString(L"КОЛ-ВО");

	while (true) //Чтение всех компонентов
	{
		if (!readComponent())
		{
			if (!readComponent()) {
				break;
			}
		}
	}

	readTablePartData();
}