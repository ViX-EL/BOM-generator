#include "DrawingPageNAG.h"

DrawingPageNAG::DrawingPageNAG(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	operatingTemperaturePattern.assign(LR"((-|\+)?\d{2,3}((\.{3}|…)\+?\d{2,3})?|AMBIENT ?\/ ?ОКР\. ?СРЕДА|Не ниже \d \/ No lower than \d)", std::regex::icase);
	operatingPressurePattern.assign(
		LR"(-?\d(,|\.)\d{1,6}((\.{2,3}|…)\.? ?(\d(,|\.)\d{1,6})?)?( \(абс\.\))?|ATM \/ АТМ\.|HYDR(?:\.|OSTATIC) \/ ГИДРОСТА?Т?\.|NO MORE \/ НЕ БОЛЕЕ \d,\d{1,6}|HOLD)");
	tracingPattern.assign(LR"(E|H|N|T|NO \/ НЕТ)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{8,9})");
	technologicalEnvironmentPattern.assign(LR"([A-Za-z]{2,4})");
	testEnvironmentPattern.assign(LR"((PLANT )?AIR ?/ ?(ТЕХНИЧЕСКИЙ )?ВОЗДУХ|FW / ПРОТИВОПОЖАРНЫЙ ВОДОПРОВОД|NO / НЕТ|WATER / ВОДА)");
	paintingSystemPattern.assign(LR"(A|B|C|T|NO \/ НЕТ)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO \/ НЕТ)");
	weldInspectionPattern.assign(LR"(\d{1,3}%? ?(?:[ 1-9а-яА-Я\(\),+\.\/%]{2,19})?|\d метода объемного контроля|NO \/ НЕТ)");
	testPressurePattern.assign(LR"(\d(,|\.)\d{1,3}|NO \/ НЕТ)");
	GOSTPipelineCategoryPattern.assign(LR"(\w{1,3}(?:-\w{1,2})?(?:(,|\.)? ?[\(\)0-9A-Za-zА-Яа-я]{1,9})?(?: \([А-Я]{3} \d{3}\))?( \/ \w{1,3} \(\w{3} \d{3}\))?)");
	designTemperaturePattern.assign(LR"(-?\d{1,3} ?( ?\/ ?|\.\.\.?)?\+?(\d{1,3})?)");
	designPressurePattern.assign(LR"(HYDR(?:\.|OSTATIC) \/ ГИДРОСТА?Т?\.|(FV|-?\d((,|\.)\d{1,9})?)?( ?\/ ?(\d(,|\.)\d{2}|FV)?)?)");
	cipherDocumentPattern.assign(LR"(GCC-NAG-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"((DN)? ?\d{2,4}(( \/ |,) ?(DN)? ?\d{2,4})?)");
	isolationPattern.assign(LR"(\d{2,3}( \d{2,3})?( \d{2,3})?( \d{2,3})?( \d{2,3})?|H|NO \/ НЕТ)");
	categoryPipelinesTRCUPattern.assign(LR"(NO \/ НЕТ|\d|[0-9A-Z() /А-Я]{10,20})");
	schemeNumberPattern.assign(LR"(GCC-NAG-DDD-\d{5}-\d{2}-\d{4}-\w{2}-\w{3}-\d{5}(, ?\d{4,5}){0,2})");
	lineNumberPattern.assign(LR"(\d{5}-\w{2}-\d{4}\/\d{3}-\w{2,4}-\d{4}-[0-9A-Z]{8,9}-\d{2})");
	stressCalculationPattern.assign(LR"(NO \/ НЕТ|YES \/ ДА)");
	isometricDrawingPattern.assign(LR"(\d{3}-\w{2,4}-\d{4}([ /0-9-]{2,5})?)");
	fileNamePattern.assign(LR"(GCC-NAG-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+\.dwg)");
}

bool DrawingPageNAG::trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, ValuesCheker::Type checkType)
{
	if (operatingTemperatureStr == L"Ambient" || operatingTemperatureStr == L"AMBIENT / ОКР.СРЕДА" || operatingTemperatureStr == L"AMBIENT/ ОКР. СРЕДА") {
		operatingTemperature = L"AMBIENT / ОКР.СРЕДА";
		return true;
	}
	if (operatingTemperatureStr == L"Не ниже +5" || operatingTemperatureStr == L"Не ниже 5") {
		operatingTemperature = L"Не ниже 5 / No lower than 5";
		return true;
	}
	return DrawingPage::trySetOperatingTemperature(operatingTemperatureStr, checkType);
}

bool DrawingPageNAG::trySetOperatingPressure(const std::wstring& operatingPressureStr, ValuesCheker::Type checkType)
{
	if (operatingPressureStr == L"АТМ." || operatingPressureStr == L"ATM / АТМ" || operatingPressureStr == L"АТМ / ATM" || operatingPressureStr == L"Atm" ||
		operatingPressureStr == L"Атм." || operatingPressureStr == L"Atm/Атм.")
	{
		operatingPressure = L"ATM / АТМ.";
		return true;
	}
	if (operatingPressureStr == L"HYDROSTATIC/ ГИДРОСТАТИЧЕСКОЕ" || operatingPressureStr == L"HYDR. / ГИДРОСТ") {
		operatingPressure = L"HYDR. / ГИДРОСТ.";
		return true;
	}
	if (operatingPressureStr.starts_with(L"Не более / No more")) {
		std::wstring newOperatingPressureStr = operatingPressureStr;
		newOperatingPressureStr.replace(0, 17, L"NO MORE / НЕ БОЛЕЕ");
		operatingPressure = newOperatingPressureStr;
		return true;
	}
	return DrawingPage::trySetOperatingPressure(operatingPressureStr, checkType);
}

bool DrawingPageNAG::trySetTracing(const std::wstring& tracingStr, ValuesCheker::Type checkType)
{
	if (tracingStr == L"NO" || tracingStr == L"NO/НЕТ") {
		tracing = L"NO / НЕТ";
		return true;
	}
	return DrawingPage::trySetTracing(tracingStr, checkType);
}

bool DrawingPageNAG::trySetTestEnvironment(const std::wstring& testEnvironmentStr, ValuesCheker::Type checkType)
{
	if (testEnvironmentStr == L"WATER/ВОДА") {
		testEnvironment = L"WATER / ВОДА";
		return true;
	}
	return DrawingPage::trySetTestEnvironment(testEnvironmentStr, checkType);
}

bool DrawingPageNAG::trySetPaintingSystem(const std::wstring& paintingSystemStr, ValuesCheker::Type checkType)
{
	if (paintingSystemStr == L"С") {
		paintingSystem = L"C";
		return true;
	}
	return DrawingPage::trySetPaintingSystem(paintingSystemStr, checkType);
}

bool DrawingPageNAG::trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, ValuesCheker::Type checkType)
{
	if (postWeldingHeatTreatmentStr == L"NO/НЕТ") {
		postWeldingHeatTreatment = L"NO / НЕТ";
		return true;
	}
	return DrawingPage::trySetPostWeldingHeatTreatment(postWeldingHeatTreatmentStr, checkType);
}

bool DrawingPageNAG::trySetDesignPressure(const std::wstring& designPressureStr, ValuesCheker::Type checkType)
{
	if (designPressureStr == L"HYDROSTATIC/ ГИДРОСТАТИЧЕСКОЕ") {
		designPressure = L"HYDR. / ГИДРОСТ.";
		return true;
	}
	return DrawingPage::trySetDesignPressure(designPressureStr, checkType);
}

bool DrawingPageNAG::trySetIsolation(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"NO" || sourceStr == L"NO/НЕТ") {
		isolation = L"NO / НЕТ";
		return true;
	}
	return DrawingPage::trySetIsolation(sourceStr, checkType);
}

bool DrawingPageNAG::trySetStressCalculation(const std::wstring& stressCalculationStr, ValuesCheker::Type checkType)
{
	if (stressCalculationStr == L"NO/НЕТ") {
		stressCalculation = L"NO / НЕТ";
		return true;
	}
	if (stressCalculationStr == L"YES/ДА" || stressCalculationStr == L"YES/ ДА") {
		stressCalculation = L"YES / ДА";
		return true;
	}
	return DrawingPage::trySetStressCalculation(stressCalculationStr, checkType);
}
