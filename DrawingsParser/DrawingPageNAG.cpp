#include "DrawingPageNAG.h"

DrawingPageNAG::DrawingPageNAG(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	operatingTemperaturePattern.assign(LR"((-|\+)?\d{2,3}((\.{3}|…)\+?\d{2,3})?|AMBIENT \/ ÎÊĞ\. ÑĞÅÄÀ|Íå íèæå \d \/ No lower than \d)");
	operatingPressurePattern.assign(LR"(-?\d(,|\.)\d{1,6}((\.{2,3}|…)\.? ?(\d(,|\.)\d{1,6})?)?( \(àáñ\.\))?|ATM \/ ÀÒÌ\.|HYDR\. \/ ÃÈÄĞÎÑÒ\.|NO MORE \/ ÍÅ ÁÎËÅÅ \d,\d{1,6}|HOLD)");
	tracingPattern.assign(LR"(E|H|N|T|NO \/ ÍÅÒ)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{8,9})");
	technologicalEnvironmentPattern.assign(LR"([A-Za-z]{2,4})");
	testEnvironmentPattern.assign(LR"((PLANT )?AIR ?/ ?(ÒÅÕÍÈ×ÅÑÊÈÉ )?ÂÎÇÄÓÕ|FW / ÏĞÎÒÈÂÎÏÎÆÀĞÍÛÉ ÂÎÄÎÏĞÎÂÎÄ|NO / ÍÅÒ|WATER / ÂÎÄÀ)");
	paintingSystemPattern.assign(LR"(A|B|C|T|NO \/ ÍÅÒ)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO \/ ÍÅÒ)");
	weldInspectionPattern.assign(LR"(\d{1,3}%?( ÏÂÊ)?|\d ìåòîäà îáúåìíîãî êîíòğîëÿ|NO \/ ÍÅÒ)");
	testPressurePattern.assign(LR"(\d(,|\.)\d{1,3}|NO \/ ÍÅÒ)");
	GOSTPipelineCategoryPattern.assign(LR"(\w{1,3}(-\w{1,2})?((,|\.)? [A-Za-zÀ-ßà-ÿ]{1,2})?( \/ \w{1,3} \(\w{3} \d{3}\))?)");
	designTemperaturePattern.assign(LR"(-?\d{1,3} ?( ?\/ ?|\.\.\.?)?\+?(\d{1,3})?)");
	designPressurePattern.assign(LR"((FV|-?\d((,|\.)\d{1,9})?)?( ?\/ ?(\d(,|\.)\d{2}|FV)?)?|HYDR\. \/ ÃÈÄĞÎÑÒ\.)");
	cipherDocumentPattern.assign(LR"(GCC-NAG-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"((DN)? ?\d{2,4}(( \/ |,) ?(DN)? ?\d{2,4})?)");
	isolationPattern.assign(LR"(\d{2,3}( \d{2,3})?( \d{2,3})?( \d{2,3})?( \d{2,3})?|H|NO \/ ÍÅÒ)");
	categoryPipelinesTRCUPattern.assign(LR"(NO \/ ÍÅÒ|\d|[0-9A-Z() /À-ß]{10,20})");
	schemeNumberPattern.assign(LR"(GCC-NAG-DDD-\d{5}-\d{2}-\d{4}-\w{2}-\w{3}-\d{5}(, ?\d{4,5}){0,2})");
	lineNumberPattern.assign(LR"(\d{5}-\w{2}-\d{4}\/\d{3}-\w{2,4}-\d{4}-[0-9A-Z]{8,9}-\d{2})");
	stressCalculationPattern.assign(LR"(NO \/ ÍÅÒ|YES \/ ÄÀ)");
	isometricDrawingPattern.assign(LR"(\d{3}-\w{2,4}-\d{4}([ /0-9-]{2,5})?)");
	fileNamePattern.assign(LR"(GCC-NAG-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+\.dwg)");
}

bool DrawingPageNAG::trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck)
{
	if (operatingTemperatureStr == L"Ambient" || operatingTemperatureStr == L"AMBIENT / ÎÊĞ.ÑĞÅÄÀ" || operatingTemperatureStr == L"AMBIENT/ ÎÊĞ. ÑĞÅÄÀ") {
		operatingTemperature = L"AMBIENT / ÎÊĞ.ÑĞÅÄÀ";
		return true;
	}
	if (operatingTemperatureStr == L"Íå íèæå +5" || operatingTemperatureStr == L"Íå íèæå 5") {
		operatingTemperature = L"Íå íèæå 5 / No lower than 5";
		return true;
	}
	return DrawingPage::trySetOperatingTemperature(operatingTemperatureStr, assertionCheck);
}

bool DrawingPageNAG::trySetOperatingPressure(const std::wstring& operatingPressureStr, bool assertionCheck)
{
	if (operatingPressureStr == L"ÀÒÌ." || operatingPressureStr == L"ATM / ÀÒÌ" || operatingPressureStr == L"ÀÒÌ / ATM" || operatingPressureStr == L"Atm" || operatingPressureStr == L"Àòì.") 
	{
		operatingPressure = L"ATM / ÀÒÌ.";
		return true;
	}
	if (operatingPressureStr == L"HYDROSTATIC/ ÃÈÄĞÎÑÒÀÒÈ×ÅÑÊÎÅ" || operatingPressureStr == L"HYDR. / ÃÈÄĞÎÑÒ") {
		operatingPressure = L"HYDR. / ÃÈÄĞÎÑÒ.";
		return true;
	}
	if (operatingPressureStr.starts_with(L"Íå áîëåå / No more")) {
		std::wstring newOperatingPressureStr = operatingPressureStr;
		newOperatingPressureStr.replace(0, 17, L"NO MORE / ÍÅ ÁÎËÅÅ");
		operatingPressure = newOperatingPressureStr;
		return true;
	}
	return DrawingPage::trySetOperatingPressure(operatingPressureStr, assertionCheck);
}

bool DrawingPageNAG::trySetTracing(const std::wstring& tracingStr, bool assertionCheck)
{
	if (tracingStr == L"NO" || tracingStr == L"NO/ÍÅÒ") {
		tracing = L"NO / ÍÅÒ";
		return true;
	}
	return DrawingPage::trySetTracing(tracingStr, assertionCheck);
}

bool DrawingPageNAG::trySetTestEnvironment(const std::wstring& testEnvironmentStr, bool assertionCheck)
{
	if (testEnvironmentStr == L"WATER/ÂÎÄÀ") {
		testEnvironment = L"WATER / ÂÎÄÀ";
		return true;
	}
	return DrawingPage::trySetTestEnvironment(testEnvironmentStr, assertionCheck);
}

bool DrawingPageNAG::trySetPaintingSystem(const std::wstring& paintingSystemStr, bool assertionCheck)
{
	if (paintingSystemStr == L"Ñ") {
		paintingSystem = L"C";
		return true;
	}
	return DrawingPage::trySetPaintingSystem(paintingSystemStr, assertionCheck);
}

bool DrawingPageNAG::trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, bool assertionCheck)
{
	if (postWeldingHeatTreatmentStr == L"NO/ÍÅÒ") {
		postWeldingHeatTreatment = L"NO / ÍÅÒ";
		return true;
	}
	return DrawingPage::trySetPostWeldingHeatTreatment(postWeldingHeatTreatmentStr, assertionCheck);
}

bool DrawingPageNAG::trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck)
{
	if (designPressureStr == L"HYDROSTATIC/ ÃÈÄĞÎÑÒÀÒÈ×ÅÑÊÎÅ") {
		designPressure = L"HYDR. / ÃÈÄĞÎÑÒ.";
		return true;
	}
	return DrawingPage::trySetDesignPressure(designPressureStr, assertionCheck);
}

bool DrawingPageNAG::trySetIsolation(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"NO" || sourceStr == L"NO/ÍÅÒ") {
		isolation = L"NO / ÍÅÒ";
		return true;
	}
	return DrawingPage::trySetIsolation(sourceStr, assertionCheck);
}

bool DrawingPageNAG::trySetStressCalculation(const std::wstring& stressCalculationStr, bool assertionCheck)
{
	if (stressCalculationStr == L"NO/ÍÅÒ") {
		stressCalculation = L"NO / ÍÅÒ";
		return true;
	}
	if (stressCalculationStr == L"YES/ÄÀ" || stressCalculationStr == L"YES/ ÄÀ") {
		stressCalculation = L"YES / ÄÀ";
		return true;
	}
	return DrawingPage::trySetStressCalculation(stressCalculationStr, assertionCheck);
}
