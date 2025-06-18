#include "SplitBuildComponentData.h"
#include "BuildComponent.h"
#include "StringUtilities.h"

#include <cassert>
#include <string_view>

SplitBuildComponentData::SplitBuildComponentData(const BuildComponent* componentPtr) : componentPtr(componentPtr)
{

}

std::wstring_view SplitBuildComponentData::getElementName() const
{
	return elementName;
}

std::wstring_view SplitBuildComponentData::getType1() const
{
	return type1;
}

std::wstring_view SplitBuildComponentData::getType2() const
{
	return type2;
}

std::wstring_view SplitBuildComponentData::getType3() const
{
	return type3;
}

std::wstring_view SplitBuildComponentData::getProductStandard() const
{
	return productStandard;
}

std::wstring_view SplitBuildComponentData::getProductStandardENG() const
{
	return productStandardENG;
}

std::wstring_view SplitBuildComponentData::getSteelGrade() const
{
	return steelGrade;
}

std::wstring_view SplitBuildComponentData::getSteelGradeENG() const
{
	return steelGradeENG;
}

std::wstring_view SplitBuildComponentData::getDiameter1() const
{
	return diameter1;
}

std::wstring_view SplitBuildComponentData::getDiameter2() const
{
	return diameter2;
}

std::wstring_view SplitBuildComponentData::getWallThickness1() const
{
	return wallThickness1;
}

std::wstring_view SplitBuildComponentData::getWallThickness2() const
{
	return wallThickness2;
}

std::wstring_view SplitBuildComponentData::getNominalDiameter1() const
{
	return nominalDiameter1;
}

std::wstring_view SplitBuildComponentData::getNominalDiameter2() const
{
	return nominalDiameter2;
}

double SplitBuildComponentData::getPressureNominal() const
{
	return pressureNominal;
}

int SplitBuildComponentData::getPressureClass() const
{
	return pressureClass;
}

std::wstring_view SplitBuildComponentData::getASMEThickness1() const
{
	return ASMEThickness1;
}

std::wstring_view SplitBuildComponentData::getASMEThickness2() const
{
	return ASMEThickness2;
}

void SplitBuildComponentData::setSteelGrade(const std::wstring& steelGrade)
{
	this->steelGrade = steelGrade;
}

void SplitBuildComponentData::parseElementName()
{
	for (const auto& [element, pattern] : elementNamePatterns) {
		if (std::regex_search(componentPtr->getDescription().begin(), componentPtr->getDescription().end(), pattern)) {
			elementName = element;
			break;
		}
	}
	if (elementName == L"Фланец")
	{
		if(componentPtr->getDescription().contains(L"Тройник") || componentPtr->getDescription().contains(L"Задвижка") || componentPtr->getDescription().contains(L"Клапан") ||
			componentPtr->getDescription().contains(L"Затвор") || componentPtr->getDescription().contains(L"Рукав") || componentPtr->getDescription().contains(L"CAMLOCK")) {
			elementName = L"-";
		}
	}
	else if (elementName.contains(L"Отвод"))
	{
		std::wstring degreeStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{2}[- ](?:сварной )?(?:\d(?:\.\d)?DN-)?\d{2,4}(?:\.\d)?x\d{1,2}(?:\.\d)?|\d{2} d\d{2,3} PN16)"));
		if (degreeStr != L"-") {
			elementName += L" " + degreeStr.substr(0, 2);
		}
		else {
			std::match_results<std::wstring_view::const_iterator> match;
			auto start = componentPtr->getDescription().begin();
			auto end = componentPtr->getDescription().end();
			int matchCount = 0;
			if (std::regex_search(start, end, match, StringUtilities::getRegex(LR"(ELBOW (\d{2})\w{2} \w{2})")))
			{
				degreeStr = match[1].str();
				elementName += L" " + degreeStr;
			}
		}
	}
	else if (elementName == L"Заглушка поворотная" && componentPtr->getDescription().contains(L"valve")) {
		elementName = L"-";
	}
	else if (elementName == L"Гайка" && componentPtr->getDescription().contains(L"Stud")) {
		elementName = L"Шпилька";
	}
	else if (elementName == L"Труба" && componentPtr->getDescription().contains(L"ОПОРА")) {
		elementName = L"Опора";
	}
}

void SplitBuildComponentData::parseType1()
{
	if (elementName == L"-") {
		return;
	}
	else if (elementName == L"Заглушка фланцевая") {
		parseType1BLIND_FLANGE();
	}
	else if (elementName == L"Заглушка") 
	{
		if (componentPtr->getDescription().contains(L"Заглушка с внешней резьбой")) {
			type1 = L"External Thread";
		}
	}
	else if (elementName == L"Крепление для труб")
	{
		if (componentPtr->getDescription().contains(L"type F")) {
			type1 = L"Type F";
		}
	}
	else if (elementName == L"Муфта")
	{
		if (componentPtr->getDescription().contains(L"MIF")) {
			type1 = L"Type MIF";
		}
	}
	else if (elementName == L"Фланец")
	{
		if (componentPtr->getDescription().contains(L"WN") || productStandard == L"ГОСТ 33259-2015" || productStandard == L"EN 1092-1"|| productStandard == L"ASME B16.(5|47)") {
			type1 = L"WN";
		}
	}
	else if (elementName == L"Труба")
	{
		if (componentPtr->getDescription().contains(L"с наружной цилиндрической резьбой")) {
			type1 = L"with external cylindrical thread";
		}
		else if (componentPtr->getDescription().contains(L"с наружной ПЭ изоляцией")) {
			type1 = L"with external PE insulation";
		}
	}
	else if (elementName == L"REDUCER ECC")
	{
		if (componentPtr->getDescription().contains(L"с наружной ПЭ изоляцией")) {
			type1 = L"with external PE insulation";
		}
	}
	else if (elementName == L"Заглушка поворотная") {
		parseType1SPECTACLE_BLIND();
	}
	/*else if (elementName == L"Опора") 
	{
		if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Опора \d{2} OCT)")); matchStr != L"-") {
			type1 = matchStr.substr(6, 2);
		}
		else if (componentPtr->getDescription().contains(L"Clamp")) 
		{
			if (componentPtr->getDescription().contains(L"Clamp 1T ")) {
				type1 = L"Clamp 1T";
			}
			else {
				type1 = L"Clamp";
			}
		}
		else if (componentPtr->getDescription().contains(L"Fixed four-stop support")) {
			type1 = L"Fixed four-stop support";
		}
		else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Fixed support for \w{3} heat tracer)")); matchStr != L"-")
		{
			size_t strBegin = matchStr.find(L"for") + 4;
			type1 = matchStr.substr(strBegin, matchStr.find(L"heat") - strBegin - 1);
		}
		else if (componentPtr->getDescription().contains(L"Spannloc")) {
			type1 = L"Spannloc";
		}
		else if (componentPtr->getDescription().contains(L"Support and guide ring")) {
			type1 = L"Support and guide ring; Precast";
		}
		else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{3}-ТО-А\d)")); matchStr != L"-") {
			type1 = L"ТО";
		}
		else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(тип \d исполнение \d)")); matchStr != L"-") {
			type1 = L"Type " + matchStr[5] + std::wstring(L"perf.") + matchStr[matchStr.size() - 1];
		}
		else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Z-D?S\d{1,2}L?-\d{2,3})")); matchStr != L"-")
		{
			size_t strBegin = matchStr.find_first_of(L'-') + 1;
			type1 = matchStr.substr(strBegin, matchStr.find_last_of(L'-') - strBegin);
		}
		else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{2,4}(?:\.\d)?-\w{2}-А)")); matchStr != L"-")
		{
			size_t strBegin = matchStr.find_first_of(L'-') + 1;
			type1 = matchStr.substr(strBegin, matchStr.find_last_of(L'-') - strBegin);
		}
	}*/
}

void SplitBuildComponentData::parseType2()
{
	if (elementName == L"-") {
		return;
	}
	else if (elementName == L"Заглушка") {
		parseType2Cap();
	}
	if (elementName == L"Заглушка фланцевая") {
		
	}
	else if (elementName.contains(L"Отвод")) {
		parseType2ELBOW();
	}
	else if (elementName == L"Фланец") {
		parseType2FLANGE();
	}
	else if (elementName == L"Концентрический переход") {
		parseType2REDUCER_CON();
	}
	else if (elementName == L"Эксцентрический переход") {
		parseType2REDUCER_ECC();
	}
	else if (elementName == L"Труба") {
		parseType2PIPE();
	}
	//else if (elementName == L"Опора") {
	//	parseType2Support();
	//}
	else if (elementName == L"Тройник") {
		parseType2Tee();
	}
}

void SplitBuildComponentData::parseType3()
{
	if (elementName == L"-") {
		return;
	}
	else if (elementName == L"Муфта")
	{
		if (componentPtr->getDescription().contains(L"PN16")) {
			type3 = L"PN16";
		}
	}
	else if (elementName.contains(L"Отвод"))
	{
		if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(PN\d{2})")); matchStr != L"-") {
			type3 = matchStr;
		}
		if (productStandard == L"GCC-NAG-DDD-12460-12-1500-TK-DSH-00003") {
			type3 = L"2.5DN R=4000";

		}
		else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(R=\d,\dDN, R=\d{4})")); matchStr != L"-") {
			type3 = matchStr;
		}
		else if (productStandard == L"ГОСТ 17375-2001" || productStandard == L"EN 10253" || productStandard == L"ТУ 3600-010-78786272-2012" ||
			searchDescriptionMatch(StringUtilities::getRegex(LR"(LR \d{2}-\d{1,2})"))!= L"-" || componentPtr->getDescription().contains(L"1.5DN")) {
			type3 = L"LR R=1.5D";
		}
		else if (searchDescriptionMatch(StringUtilities::getRegex(LR"(SR \d{2}-\d{1,2})")) != L"-" || (componentPtr->getDescription().contains(L"Опросный") &&
			searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{2} \d{4}x\d{1,2})")) != L"-") || componentPtr->getDescription().contains(L"R=DN") ||
			componentPtr->getDescription().contains(L"1DN")) {
			type3 = L"SR R=DN";
		}
		else if (componentPtr->getDescription().contains(L"flanged on one side")) {
			type3 = L"Flanged on one side";
		}
	}
	else if (elementName == L"Фланец")
	{
		if (componentPtr->getDescription().contains(L"long")) {
			type3 = L"LONG";
		}
	}
	else if (elementName == L"MIFV" || elementName == L"MIMV" || elementName == L"Переходная муфта")
	{
		if  (componentPtr->getDescription().contains(L"PN16")) {
			type3 = L"PN16";
		}
	}
	else if (elementName == L"Тройник")
	{
		if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(PN\d{1,2})")); matchStr != L"-") {
			type3 = matchStr;
		}
		else if (componentPtr->getDescription().contains(L"Flange tee")) {
			type3 = L"Flange tee";
		}
		else if (componentPtr->getDescription().contains(L"Type B")) {
			type3 = L"Flange tee";
		}
	}
	else if (elementName == L"Эксцентрический переход") {
		parseType3REDUCER_ECC();
	}
}

void SplitBuildComponentData::parseType2Tee()
{
	if (searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{1,4}(?:\.\d)?x\d{1,4}(?:\.\d)?-\d{1,4}(?:\.\d)?x\d{1,4}(?:\.\d)?)")) != L"-") {
		type2 = L"RED";
	}
}

void SplitBuildComponentData::parseType1Tee()
{
	if (productStandard == L"ГОСТ 17376-2001" || componentPtr->getDescription().contains(L"SMLS")) {
		type1 = L"SMLS";
	}
	else if (productStandard == L"ОСТ 36-23-77" || productStandard == L"ТУ 3600-010-78786272-2007" || productStandard == L"ТУ 3600-010-78786272-2012") {
		type1 = L"Stamp welded";
	}
	else if (componentPtr->getDescription().contains(L"TIF")) {
		type1 = L"TIF";
	}
	else if (componentPtr->getDescription().contains(L"TSH")) {
		type1 = L"TSh";
	}
	else if (componentPtr->getDescription().contains(L"TT-1")) {
		type1 = L"TT-1";
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"([Tt]ee (?:[\w ]+)? (?:TS|ТС)|Welded)")); matchStr != L"-" ||
		searchMatch(StringUtilities::getRegex(L"ОСТ 36-24-77")) != L"-") {
		type1 = L"Welded";
	}
}

//void SplitBuildComponentData::parseType2Support()
//{
//	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{2}-\d{2}-\d)")); matchStr != L"-") {
//		type1 = matchStr;
//	}
//	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{2,4}(?:\.\d)?-\w{2}-АS?C?\d{1,2})")); matchStr != L"-")
//	{
//		size_t strBegin = matchStr.find_last_of(L'-') + 1;
//		type1 = matchStr.substr(strBegin, matchStr.size() - strBegin);
//	}
//	else if (componentPtr->getDescription().contains(L"ВП-Б2")) {
//		type1 = L"B2";
//	}
//	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Z-D?S\d{1,2}L?-\d{2,4}-\w(?:\d{3})?(?:-[^A\d]{1,2})?)")); matchStr != L"-")
//	{
//		size_t strBegin{ 0 };
//		for (size_t i = 0; i < 3; i++) {
//			strBegin = matchStr.find_first_of(L'-', strBegin + 1) + 1;
//		}
//		type1 = matchStr.substr(strBegin, matchStr.size() - strBegin);
//	}
//	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Support and guide ring DN\d{2,3}х\d{2,3})")); matchStr != L"-")
//	{
//		size_t strBegin = matchStr.find_last_of(L"DN") - 1;
//		type1 = matchStr.substr(strBegin, matchStr.size() - strBegin);
//	}
//	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Saddle HS\d)")); matchStr != L"-")
//	{
//		size_t strBegin = matchStr.find_last_of(L"HS") - 1;
//		type1 = matchStr.substr(strBegin, matchStr.size() - strBegin);
//	}
//	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(
//		LR"(MPN-RC \d? ?(?:\d{1,2}\/\d{1,2}|\d)\"? ?A? #\d{6}  \(with rod M\d{2} L=\d{2,3}(?:\.\d{1,2})?mm)")); matchStr != L"-")
//	{
//		size_t beginErase = matchStr.find_first_of(L"#") - 1;
//		type1 = matchStr.erase(beginErase, matchStr.find_first_of(L"(") + 1 - beginErase);
//		type1.insert(type1.find_first_of(L"with"), L" ");
//	}
//	else if (componentPtr->getDescription().contains(L"SC-RK")) {
//		type1 = L"SC-RK";
//	}
//	else if (componentPtr->getDescription().contains(L"TС-661.00.00-12")) {
//		type1 = L"TС-661.00.00-12";
//	}
//	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(П \d{3}-\d{3,4}-\d \(Ду\d{4}\))")); matchStr != L"-") {
//		type1 = matchStr;
//	}
//}

void SplitBuildComponentData::parseType1BLIND_FLANGE()
{
	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{1,2}-\d{2,4}-\d{1,2}[,\.]?\d?)")); matchStr != L"-")
	{
		matchStr = matchStr.substr(0, matchStr.find_first_of(L'-'));
		int typeNumber = stoi(matchStr);
		switch (typeNumber)
		{
		case 1:
			type1 = L"TYPE-1";
			break;
		case 4:
			type1 = L"TYPE-4";
			break;
		case 3:
			type1 = L"TYPE-3";
			break;
		case 5:
			type1 = L"TYPE-5";
			break;
		}
	}
	else
	{
		if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(PN \d{2} ([\d ]+) GEORG FISCHER)"), 3); matchStr != L"-") {
			type1 = StringUtilities::removeSpaces(matchStr);
		}
		else if (searchDescriptionMatch(StringUtilities::getRegex(LR"(class #?\d{2,4} RF)")) != L"-") {
			type1 = L"TYPE-RF";
		}
		else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Flangespacer [\d\-/,]\-B\-)")); matchStr != L"-") {
			type1 = L"TYPE-B";
		}
		else
		{
			if (searchDescriptionMatch(StringUtilities::getRegex(LR"(class \d{2,4} RTJ)")) != L"-") {
				type1 = L"TYPE-RTJ";
			}
		}
	}
}

void SplitBuildComponentData::parseType2BLIND_FLANGE()
{
	if (componentPtr->getDescription().contains(L"spacer"))
	{
		type2 = L"Spacer";
	}
}

void SplitBuildComponentData::parseType2ELBOW()
{
	if (componentPtr->getDescription().contains(L"GIF")) {
		type2 = L"GIF";
	}
	if (productStandard == L"ГОСТ 17375-2001" || productStandard == L"ASME B16.9" || productStandard == L"ГОСТ 30753-2001") {
		type2 = L"SMLS";
		if (componentPtr->getDescription().contains(L"external PE insulation")) {
			type2 += L"; external PE insulation";
		}
	}
	if (componentPtr->getDescription().contains(L"Welded") || (componentPtr->getDescription().contains(L"Опросный  лист") && 
		searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{2} \d{4}x\d{2})")) != L"-") || searchDescriptionMatch(StringUtilities::getRegex(LR"(GCC-NAG-DDD[-\d]+TK-DSH)")) != L"-") {
		type2 = L"Welded";
		if (componentPtr->getDescription().contains(L"external PE insulation")) {
			type2 += L"; external PE insulation";
		}
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(SDR \d{1,2})")); matchStr != L"-") {
		type2 = matchStr;
	}
	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(PN \d{2} ([\d ]+) GEORG FISCHER)"), 3); matchStr != L"-") {
		type1 = StringUtilities::removeSpaces(matchStr);
	}
	else if (productStandard == L"TU3600-010-78786272-2012") {
		type2 = L"ОК";
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(OKShS \d{2}-1(?:\.\d)?DN-\d{2,4}x\d{1,2})")); matchStr != L"-" ||
		componentPtr->getDescription().contains(L"Stamp-Welded")) {
		type2 = L"Stamp Welded ";
	}
}

void SplitBuildComponentData::parseType2FLANGE()
{
	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(11-1-\w)")); matchStr != L"-") {
		type2 = matchStr;
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{3} RF)")); matchStr != L"-") {
		type2 = L"RF";
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{3,4} RTJ)")); matchStr != L"-") {
		type2 = L"RTJ";
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(1/01/\w\d?)")); matchStr != L"-") {
		type2 = matchStr;
	}
}

void SplitBuildComponentData::parseType2PIPE()
{
	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(SDR\d{2}(?:\.\d)?)")); matchStr != L"-") {
		type2 = matchStr;
	}
	else if (productStandard == L"ГОСТ 8734-75") {
		type2 = L"Seamless cold-deformed";
	}
	else if (productStandard == L"ГОСТ 8732-78" || productStandard == L"ГОСТ 9940-81") {
		type2 = L"Seamless hot-deformed";
	}
	else if (componentPtr->getDescription().contains(L"SMLS")) {
		type2 = L"SMLS";
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(сварная|Pipe EFW)")); matchStr != L"-" || productStandard == L"EN 10217-7" ||
		productStandard == L"ГOCT 31447-2012" || productStandard == L"ГОСТ 10704-91" || productStandard == L"ГОСТ 31447-2012" || productStandard == L"ТУ 1381-001-62594197-2011") {
		type2 = L"welded steel pipes";
	}
	else if (componentPtr->getDescription().contains(L"наружной ПЭ")) {
		type2 = L"welded with external PE insulation";
	}
	else if (componentPtr->getDescription().contains(L"наружной цилиндрической")) {
		type2 = L"welded with external cylindrical thread";
	}
}

void SplitBuildComponentData::parseType2REDUCER_ECC()
{
	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(SDR \d{2})")); matchStr != L"-") {
		type2 = matchStr;
	}
	else if (componentPtr->getDescription().contains(L"PESH")) {
		type2 = L"Shtamped";
	}
	else if (componentPtr->getDescription().contains(L"Reducer PET")) {
		type2 = L"Chiseled PT";
	}
	else if (productStandard == L"ГОСТ 17378-2001" || productStandard == L"ASME B16.9") {
		type2 = L"SMLS";
	}
	else if (searchDescriptionMatch(StringUtilities::getRegex(LR"(Welded|сварной|ПЭС)")) != L"-" || productStandard == L"ОСТ 36-22-77" ||
		(searchDescriptionMatch(StringUtilities::getRegex(LR"(Опросный  ?лист)")) != L"-" && searchDescriptionMatch(StringUtilities::getRegex(LR"(Reducer  ?Э \d{4}x\d{2})")) != L"-")) {
		type2 = L"Welded";
	}
}

void SplitBuildComponentData::parseType1SPECTACLE_BLIND()
{
	if (productStandard == L"339447 VO") {
		type1 = productStandard;
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(class \d{2,3} RF)")); matchStr != L"-") {
		type1 = L"RF";
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"((?:\d|\(I\))[-| ]\d{2}-\d{2}(?:[,\.]\d{1,2})?)")); matchStr != L"-") {
		type1 = L"TYPE ";
		(matchStr.at(0) != L'(') ? type1 += matchStr.at(0) : type1 += matchStr.at(1);
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(ZPK \d{2,3}-\d{1,2}(?:,\d)-\d)")); matchStr != L"-") {
		type1 = L"ZPK-" + matchStr.at(matchStr.size() - 1);
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(ZPP \d{2,3}-\d{1,2}(?:,\d)-\d)")); matchStr != L"-") {
		type1 = L"ZPP-" + matchStr.at(matchStr.size() - 1);
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(ZPSH \d{2,3}-\d{1,2}(?:,\d)-\d)")); matchStr != L"-") {
		type1 = L"ZPSH-" + matchStr.at(matchStr.size() - 1);
	}
}

void SplitBuildComponentData::parseType2REDUCER_CON()
{
	if (componentPtr->getDescription().contains(L"PKT") || componentPtr->getDescription().contains(L"точеный")) {
		type2 = L"Chiseled PT";
	}
	else if (componentPtr->getDescription().contains(L"ПКШ")) {
		type2 = L"Shtamped";
	}
	else if (searchDescriptionMatch(StringUtilities::getRegex(LR"(Reducer P\*? K)")) != L"-" || componentPtr->getDescription().contains(L"SMLS")) {
		type2 = L"SMLS";
	}
	if (componentPtr->getDescription().contains(L"сварной") || productStandard == L"ОСТ 34.10.753-97") {
		type2 = L"welded";
	}
}

void SplitBuildComponentData::parseType3REDUCER_ECC()
{
	if (searchDescriptionMatch(StringUtilities::getRegex(LR"(Non-?standar(?:d|t))")) != L"-") {
		type3 = L"Non-standart";
	}
	else if (componentPtr->getDescription().contains(L"наружной")) {
		type3 = L"With external PE insulation";
	}
}

void SplitBuildComponentData::parseType2Cap()
{
	if (componentPtr->getDescription().contains(L"PVC-U")) {
		type2 = L"PVC-U";
	}
	else if (componentPtr->getDescription().contains(L"ПВДФ")) {
		type2 = L"PVDF";
	}
}

void SplitBuildComponentData::parseProductStandard()
{
	std::vector<std::wstring> patternStrings {
		LR"(ASME B\d{2}\.\d{1,2}M?)",
		LR"(EN [\d\-]+)",
		LR"(GCC-\w{3}-\w{2,3}-\d{5}-\d{2}-\d{4}-\w{2,4}-\w{3}-\d{5})",
		LR"(ISO \d{4,5})",
		LR"(MSS SP-\d{2})",
		LR"(N \d{4}-\d)",
		LR"(АТК \d{2}[\.-]\d{2,3}[\.-]\d{1,2}-\d{2})",
		LR"(Т-(?:\d{6}-ТМ|ММ-\d{2})-\d{2}-\d{2}-\d{2,3})",
	};
	for (auto patternStr : patternStrings)
	{
		if (std::wstring matchStr = searchMatch(StringUtilities::getRegex(patternStr)); matchStr != L"-")
		{
			productStandard = matchStr;
			productStandardENG = matchStr;
			break;
		}
	}
	
	if (productStandard != L"-") {
		return;
	}

	if (componentPtr->getDescription().contains(L"339447 VO"))
	{
		productStandardENG = L"339447 VO";
		productStandard = productStandardENG.erase(productStandardENG.size() - 3, 2) + L"ВО";
	}
	if (std::wstring matchStr = searchMatch(StringUtilities::getRegex(LR"(OCT \d{2}[\.-]\d{2,4}[\.|-]\d{2,3}-?\d?\d?)")); matchStr != L"-")
	{
		productStandard = matchStr;
		productStandardENG = L"OST" + matchStr.erase(0, 3);
	}
	else if (std::wstring matchStr = searchMatch(StringUtilities::getRegex(LR"(СК \d{4}-\d{2}-\d{3}\.\d{2})")); matchStr != L"-")
	{
		productStandard = matchStr;
		productStandardENG = L"SK" + matchStr.erase(0, 2);
	}
	else if (std::wstring matchStr = searchMatch(StringUtilities::getRegex(LR"(ГОСТ Р? ?\d{4,5}-\d{2,4})")); matchStr != L"-")
	{
		productStandard = matchStr;
		productStandardENG = L"GOST" + matchStr.erase(0, 4);
	}
	else if (std::wstring matchStr = searchMatch(StringUtilities::getRegex(LR"(ТУ \d{4}-\d{3}-\d{8}-\d{4})")); matchStr != L"-")
	{
		productStandard = matchStr;
		productStandardENG = L"TU" + matchStr.erase(0, 2);
	}
}

void SplitBuildComponentData::parseSteelGrade()
{
	if (steelGrade != L"-") {
		return;
	}

	if (componentPtr->getDescription().contains(L"08Х18Н10Т"))
	{
		steelGrade = L"08Х18Н10Т";
		steelGradeENG = StringUtilities::transliterate(steelGrade);
	}
	else if (componentPtr->getDescription().contains(L"12Х18Н10Т"))
	{
		steelGrade = L"12Х18Н10Т";
		steelGradeENG = StringUtilities::transliterate(steelGrade);
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(S?A-516 Gr70)")); matchStr != L"-")
	{
		steelGrade = matchStr;
		steelGradeENG = matchStr;
	}
	else if (componentPtr->getDescription().contains(L"PE 100"))
	{
		steelGrade = L"ПЭ 100";
		steelGradeENG = L"PE 100";
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(1.4\d01/1.4\d0\d)")); matchStr != L"-") {
		steelGrade = matchStr;
		steelGradeENG = matchStr;
	}
	else if (componentPtr->getDescription().contains(L"17Г1С-12"))
	{
		steelGrade = L"17Г1С-12";
		steelGradeENG = L"17G1S-12";
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(
		LR"((?:ASTM )?A\d{3} (?:GR\.? |Gr\.)?(?:F\d{2,3})?(?:LF\d)?(?:CC\d{2})?(?: CL\.?\d{1,2})?(?:WPL?\d{1,2})?(?:TP\d{3})?(\d{1,2}\w?)?(?:P\d{2})?)")); matchStr != L"-")
	{
		steelGrade = matchStr;
		steelGradeENG = matchStr;
	}
	else if (componentPtr->getDescription().contains(L"PP FRP")) {
		steelGrade = L"PP FRP";
		steelGradeENG = L"PP FRP";
	}
	else if (componentPtr->getDescription().contains(L"PE-RT")) {
		steelGrade = L"PE-RT";
		steelGradeENG = L"PE-RT";
	}
	else if (componentPtr->getDescription().contains(L"НПВХ") || componentPtr->getDescription().contains(L"PVC-U"))
	{
		steelGrade = L"НПВХ";
		steelGradeENG = L"PVC-U";
	}
	else if (componentPtr->getDescription().contains(L"ПВДФ") || componentPtr->getDescription().contains(L"PVDF"))
	{
		steelGrade = L"ПВДФ";
		steelGradeENG = L"PVDF";
	}
	else if (componentPtr->getDescription().contains(L"ПВХ") || componentPtr->getDescription().contains(L"PVC"))
	{
		steelGrade = L"ПВХ";
		steelGradeENG = L"PVC";
	}
	else if (componentPtr->getDescription().contains(L"Ст3пс"))
	{
		steelGrade = L"Cт3пс";
		steelGradeENG = StringUtilities::transliterate(steelGrade);
	}
	else if (componentPtr->getDescription().contains(L"Ст3сп5"))
	{
		steelGrade = L"Ст3сп5";
		steelGradeENG = StringUtilities::transliterate(steelGrade);
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"([Сс]т\.?(?:аль )?20)")); matchStr != L"-")
	{
		steelGrade = L"Сталь 20";
		steelGradeENG = L"Steel 20";
	}
}

void SplitBuildComponentData::parseDiameter1()
{
/*	if (elementName == L"Опора")
	{
		if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Support \d{2,4}(?:\.\d{1,2})?)")); matchStr != L"-") {
			diameter1 = stod(matchStr.erase(0, 7));
		}
		else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Saddle HS\d-\d{2,3}(?:\.\d)?)")); matchStr != L"-")
		{
			size_t beginSubStrIndex = matchStr.find_first_of(L'-') + 1;
			diameter1 = stod(matchStr.substr(beginSubStrIndex, matchStr.size() - beginSubStrIndex));
		}
		else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Clamp 1T \d)")); matchStr != L"-")
		{
			size_t beginSubStrIndex{ 0 };
			for (size_t i = 0; i < 2; i++) {
				beginSubStrIndex = matchStr.find_first_of(L' ', beginSubStrIndex + 1) + 1;
			}
			diameter1 = stod(matchStr.substr(beginSubStrIndex, matchStr.size() - beginSubStrIndex));
		}
	}
	else*/ if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{1,4}(?:\.\d)?x\d{1,4}(?:\.\d)?)")); matchStr != L"-")
	{
		if (elementName == L"Фланец") {
			return;
		}
		if (elementName == L"Накладка") 
		{
			if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(Pad \w \d{2,3})")); matchStr != L"-")
			{
				size_t beginSubStrIndex = matchStr.find_last_of(L' ');
				diameter1 = matchStr.substr(beginSubStrIndex, matchStr.size() - beginSubStrIndex);
				return;
			}
		}
		diameter1 = matchStr.substr(0, matchStr.find_first_of(L'x'));
		if (diameter1 == nominalDiameter1) {
			diameter1 = L"-";
		}
	}
}

void SplitBuildComponentData::parseDiameter2()
{
	if (elementName == L"-") {
		return;
	}
	else if (elementName == L"Концентрический переход" || elementName == L"Эксцентрический переход" || (elementName == L"Тройник" && type2 == L"RED") || elementName == L"Накладка")
	{
		if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{1,4}(?:\.\d)?x\d{1,4}(?:\.\d)?)"), 1); matchStr != L"-") {
			size_t endSubStrIndex = matchStr.find_first_of(L'x');
			diameter2 = matchStr.substr(0, endSubStrIndex);
			if (diameter2 == diameter1 || diameter2 == nominalDiameter2) {
				diameter2 = L"-";
			}
		}
		else if (elementName == L"Концентрический переход" || elementName == L"Эксцентрический переход")
		{
			if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(d{1,4}(?:\.\d)?xd{1,4}(?:\.\d)?xd{1,4}(?:\.\d)?)")); matchStr != L"-")
			{
				diameter2 = matchStr.substr(matchStr.find_first_of(L'x') + 1, matchStr.find_last_of(L'x') - matchStr.find_first_of(L'x') - 1);
			}
		}
	}
}

void SplitBuildComponentData::parseNominalDiameter1()
{
	if (componentPtr->getNominalDiameter().contains(L'x')) {
		nominalDiameter1 = componentPtr->getNominalDiameter().substr(0, componentPtr->getNominalDiameter().find(L'x'));
	}
	else {
		nominalDiameter1 = componentPtr->getNominalDiameter();
	}
}

void SplitBuildComponentData::parseNominalDiameter2()
{
	if (componentPtr->getNominalDiameter().contains(L'x'))
	{
		size_t strBegin = componentPtr->getNominalDiameter().find(L'x') + 1;
		nominalDiameter2 = componentPtr->getNominalDiameter().substr(strBegin, componentPtr->getNominalDiameter().size() - strBegin);
	}
}

void SplitBuildComponentData::parseWallThickness1()
{
	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{1,4}(?:\.\d)?x\d{1,4}(?:\.\d)?)")); matchStr != L"-")
	{
		size_t beginSubStrIndex = matchStr.find_first_of(L'x');
		wallThickness1 = matchStr.substr(beginSubStrIndex + 1, matchStr.size() - beginSubStrIndex);
	}
	else if (elementName == L"Концентрический переход" || elementName == L"Эксцентрический переход")
	{
		if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(d{1,4}(?:\.\d)?xd{1,4}(?:\.\d)?xd{1,4}(?:\.\d)?)")); matchStr != L"-")
		{
			size_t beginSubStrIndex = matchStr.find_last_of(L'x');
			wallThickness1 = matchStr.substr(beginSubStrIndex + 1, matchStr.size() - beginSubStrIndex);
			wallThickness2 = wallThickness1;
		}
	}
}

void SplitBuildComponentData::parseWallThickness2()
{
	if (elementName == L"-") {
		return;
	}
	else if (elementName == L"Концентрический переход" || elementName == L"Эксцентрический переход" || (elementName == L"Тройник" && type2 == L"RED"))
	{
		if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{1,4}(?:\.\d)?x\d{1,4}(?:\.\d)?-\d{1,4}(?:\.\d)?x\d{1,4}(?:\.\d)?)"));
			matchStr != L"-")
		{
			size_t beginSubStrIndex = matchStr.find_last_of(L'x');
			wallThickness2 = matchStr.substr(beginSubStrIndex + 1, matchStr.size() - beginSubStrIndex);
		}
	}
}

void SplitBuildComponentData::parsePressureNominal()
{
	if (elementName != L"Заглушка фланцевая" && elementName != L"Фланец" && elementName != L"Заглушка поворотная") {
		return;
	}
	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(\d{1,2}-\d{1,2}-\d{1,2}[\.,]?\d?)")); matchStr != L"-")
	{
		size_t beginSubStrIndex{ 0 };
		std::wstring pressureNominalStr;
		if (elementName != L"Заглушка фланцевая")
		{
			beginSubStrIndex = matchStr.find_last_of(L'-');
			pressureNominalStr = matchStr.substr(beginSubStrIndex + 1, matchStr.size() - beginSubStrIndex);
		}
		else
		{
			beginSubStrIndex = matchStr.find_first_of(L'-');
			pressureNominalStr = matchStr.substr(beginSubStrIndex + 1, matchStr.find_last_of(L'-') - beginSubStrIndex);
		}
		if (pressureNominalStr.contains(L','))
		{
			size_t separatorIndex = pressureNominalStr.find(L',');
			pressureNominalStr.replace(separatorIndex, 1, L".");
		}
		pressureNominal = stod(pressureNominalStr);
		if (pressureNominalStr.contains(L'.')) {
			pressureNominal *= 10;
		}
	}
}

void SplitBuildComponentData::parsePressureClass()
{
	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(class \d{3,4})")); matchStr != L"-")
	{
		size_t beginSubStrIndex = matchStr.find_last_of(L' ');
		pressureClass = stoi(matchStr.substr(beginSubStrIndex + 1, matchStr.size() - beginSubStrIndex));
	}
}

void SplitBuildComponentData::parseASMEThickness1()
{
	if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(SCH \d{2,3}S?)")); matchStr != L"-") {
		ASMEThickness1 = matchStr;
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(STD \d{2,3}S?)")); matchStr != L"-")
	{
		ASMEThickness1 = L"STD";
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(XS \d{2,3}S?)")); matchStr != L"-")
	{
		if (matchStr == L"XS 40") {
			ASMEThickness1 = L"XS 40";
		} 
		else {
			ASMEThickness1 = L"XS";
		}
	}
	else if (std::wstring matchStr = searchDescriptionMatch(StringUtilities::getRegex(LR"(XXS \d{1,2})")); matchStr != L"-") {
		ASMEThickness1 = L"XXS";
	}
}

void SplitBuildComponentData::parseASMEThickness2()
{
	if (ASMEThickness1 != L"-" && diameter2 != L"-") {
		ASMEThickness2 = ASMEThickness1;
	}
}

void SplitBuildComponentData::parse()
{
	parseElementName();
	parseProductStandard();
	parseType1();
	parseType2();
	parseType3();
	parseSteelGrade();
	parseNominalDiameter1();
	parseNominalDiameter2();
	parseDiameter1();
	parseDiameter2();
	parseWallThickness1();
	parseWallThickness2();
	parsePressureNominal();
	parsePressureClass();
	parseASMEThickness1();
	parseASMEThickness2();
}

std::wstring SplitBuildComponentData::searchDescriptionMatch(const std::wregex& pattern, int matchIndex)
{
	std::match_results<std::wstring_view::const_iterator> match;
	auto start = componentPtr->getDescription().begin();
	auto end = componentPtr->getDescription().end();
	int matchCount = 0;
	while (std::regex_search(start, end, match, pattern))
	{
		matchCount++;
		start = match[0].second;
		if (matchCount - 1 == matchIndex) {
			break;
		}
	}
	if (match.size() == 0) {
		return L"-";
	}
	return match.str();
}

std::wstring SplitBuildComponentData::searchDocumentMatch(const std::wregex& pattern)
{
	std::match_results<std::wstring_view::const_iterator> match;
	if (std::regex_search(componentPtr->getDocument().begin(), componentPtr->getDocument().end(), match, pattern)) {
		return match.begin()->str();
	}
	return L"-";
}

std::wstring SplitBuildComponentData::searchMatch(const std::wregex& pattern)
{
	if (componentPtr->getDocument() != L"-") {
		return searchDocumentMatch(pattern);
	}
	else {
		return searchDescriptionMatch(pattern);
	}
}
