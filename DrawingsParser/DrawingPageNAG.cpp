#include "DrawingPageNAG.h"

DrawingPageNAG::DrawingPageNAG()
{
	operatingTemperaturePattern.assign(LR"((-|\+)?\d{2,3}(\.\.\.\+?\d{2,3})?|AMBIENT \/ нйп\. япедю|мЕ МХФЕ \d \/ No lower than \d)");
	operatingPressurePattern.assign(LR"(-?\d,\d{1,6}(\.\.\.\d,\d{1,6})?|ATM \/ юрл\.|HYDR\. \/ цхдпняр|NO MORE \/ ме анкее \d,\d{1,6}|HOLD)");
	tracingPattern.assign(LR"(E|H|N|T|NO \/ мер)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{8,9})");
	technologicalEnvironmentPattern.assign(LR"([A-Z]{2,4})");
	testEnvironmentPattern.assign(LR"((PLANT )?AIR ?/ ?(реумхвеяйхи )?бнгдсу|FW / опнрхбнонфюпмши бнднопнбнд|NO / мер|WATER / бндю)");
	paintingSystemPattern.assign(LR"(A|B|C|T|NO \/ мер)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO \/ мер)");
	weldInspectionPattern.assign(LR"(\d{1,3}%?( обй)?|\d ЛЕРНДЮ НАЗЕЛМНЦН ЙНМРПНКЪ|NO \/ мер)");
	testPressurePattern.assign(LR"(\d,\d{1,3}|NO \/ мер)");
	GOSTPipelineCategoryPattern.assign(LR"(\w{1,3}(-\w{1,2})?((,|\.) \w{1,2})?( \/ \w{1,3} \(\w{3} \d{3}\))?)"); //[A-Za-zю-ъЮ-Ъ .,/()0-9-]{4,22}
	designTemperaturePattern.assign(LR"(-?\d{1,3} ?( ?\/ ?|\.\.\.?)?\+?(\d{1,3})?)");
	designPressurePattern.assign(LR"((FV|-?\d(,\d{1,9})?)?( ?\/ ?(\d(,|\.)\d{2}|FV)?)?|HYDR\. \/ цхдпняр\.)");
	cipherDocumentPattern.assign(LR"(GCC-NAG-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"(DN ?\d{2,4}(( \/ |,) ?(DN)? ?\d{2,4})?)");
	isolationPattern.assign(LR"(\d{2,3}( \d{2,3})?( \d{2,3})?( \d{2,3})?( \d{2,3})?|H|NO \/ мер)");
	categoryPipelinesTRCUPattern.assign(LR"(NO \/ мер|\d|[0-9A-Z() /ю-ъ]{10,20})");
	schemeNumberPattern.assign(LR"(GCC-NAG-DDD-\d{5}-\d{2}-\d{4}-\w{2}-\w{3}-\d{5}(, ?\d{5}){0,2})");
	lineNumberPattern.assign(LR"(\d{5}-\w{2}-\d{4}\/\d{3}-\w{2,4}-\d{4}-[0-9A-Z]{9}-\d{2})");
	stressCalculationPattern.assign(LR"(NO \/ мер|YES \/ дю)");
	isometricDrawingPattern.assign(LR"(\d{3}-\w{2,4}-\d{4}([ /0-9-]{2,5})?)");
	fileNamePattern.assign(LR"(GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+\.dwg)");
}

//bool DrawingPageNAG::tryAddComponent(const std::wstring& componentNumberStr)
//{
//	if (std::regex_match(componentNumberStr, positionNumberPattern))
//{
//	components.emplace_back(std::make_shared<BuildComponentNAG>(new BuildComponentNAG(componentNumberStr)));
//	return true;
//}
//	return false;
//}

bool DrawingPageNAG::trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck)
{
	std::wstring newOperatingTemperatureStr;
	if (operatingTemperatureStr == L"Ambient" || operatingTemperatureStr == L"AMBIENT / нйп.япедю" || operatingTemperatureStr == L"AMBIENT/ нйп. япедю") {
		newOperatingTemperatureStr = L"AMBIENT / нйп.япедю";
	}
	if (operatingTemperatureStr == L"мЕ МХФЕ +5" || operatingTemperatureStr == L"мЕ МХФЕ 5") {
		newOperatingTemperatureStr = L"мЕ МХФЕ 5 / No lower than 5";
	}

	if (newOperatingTemperatureStr != L"") {
		return DrawingPage::trySetOperatingTemperature(newOperatingTemperatureStr, assertionCheck);
	}
	return DrawingPage::trySetOperatingTemperature(operatingTemperatureStr, assertionCheck);
}

bool DrawingPageNAG::trySetOperatingPressure(const std::wstring& operatingPressureStr, bool assertionCheck)
{
	std::wstring newOperatingPressureStr;
	if (newOperatingPressureStr == L"юрл." || newOperatingPressureStr == L"ATM / юрл" || newOperatingPressureStr == L"Atm") {
		newOperatingPressureStr = L"ATM / юрл.";
	}
	if (newOperatingPressureStr == L"HYDROSTATIC/ цхдпнярюрхвеяйне") {
		newOperatingPressureStr = L"HYDR. / цхдпняр";
	}
	if (newOperatingPressureStr.starts_with(L"мЕ АНКЕЕ / No more")) {
		newOperatingPressureStr.replace(0, 17, L"NO MORE / ме анкее");
	}
	if (newOperatingPressureStr != L"") {
		return DrawingPage::trySetOperatingPressure(newOperatingPressureStr, assertionCheck);
	}
	return DrawingPage::trySetOperatingPressure(operatingPressureStr, assertionCheck);
}

bool DrawingPageNAG::trySetTracing(const std::wstring& tracingStr, bool assertionCheck)
{
	if (tracingStr == L"NO" || tracingStr == L"NO/мер") {
		return DrawingPage::trySetOperatingPressure(L"NO / мер", assertionCheck);
	}
	return DrawingPage::trySetTracing(tracingStr, assertionCheck);
}

bool DrawingPageNAG::trySetTestEnvironment(const std::wstring& testEnvironmentStr, bool assertionCheck)
{
	if (testEnvironmentStr == L"WATER/бндю") {
		return DrawingPage::trySetOperatingPressure(L"WATER / бндю", assertionCheck);
	}
	return DrawingPage::trySetTestEnvironment(testEnvironmentStr, assertionCheck);
}

bool DrawingPageNAG::trySetPaintingSystem(const std::wstring& paintingSystemStr, bool assertionCheck)
{
	if (paintingSystemStr == L"я") {
		return DrawingPage::trySetPaintingSystem(L"C", assertionCheck);
	}
	return DrawingPage::trySetPaintingSystem(paintingSystemStr, assertionCheck);
}

bool DrawingPageNAG::trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, bool assertionCheck)
{
	if (postWeldingHeatTreatmentStr == L"NO/мер") {
		DrawingPage::trySetPostWeldingHeatTreatment(L"NO / мер", assertionCheck);
	}
	return DrawingPage::trySetPostWeldingHeatTreatment(postWeldingHeatTreatmentStr, assertionCheck);
}

bool DrawingPageNAG::trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck)
{
	if (designPressureStr == L"HYDROSTATIC/ цхдпнярюрхвеяйне") {
		DrawingPage::trySetDesignPressure(L"HYDR. / цхдпняр.", assertionCheck);
	}
	return DrawingPage::trySetDesignPressure(designPressureStr, assertionCheck);
}

bool DrawingPageNAG::trySetStressCalculation(const std::wstring& stressCalculationStr, bool assertionCheck)
{
	std::wstring newStressCalculationStr;
	if (newStressCalculationStr == L"NO/мер") {
		newStressCalculationStr = L"NO / мер";
	}
	if (newStressCalculationStr == L"YES/дю" || newStressCalculationStr == L"YES/ дю") {
		newStressCalculationStr = L"YES / дю";
	}
	if (newStressCalculationStr != L"") {
		return DrawingPage::trySetStressCalculation(newStressCalculationStr, assertionCheck);
	}
	return DrawingPage::trySetStressCalculation(stressCalculationStr, assertionCheck);
}
