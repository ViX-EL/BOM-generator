#include "DrawingPageNAG.h"

DrawingPageNAG::DrawingPageNAG(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	operatingTemperaturePattern.assign(LR"((-|\+)?\d{2,3}((\.{3}|�)\+?\d{2,3})?|AMBIENT \/ ���\. �����|�� ���� \d \/ No lower than \d)");
	operatingPressurePattern.assign(LR"(-?\d(,|\.)\d{1,6}((\.{2,3}|�)\.? ?(\d(,|\.)\d{1,6})?)?( \(���\.\))?|ATM \/ ���\.|HYDR\. \/ �������\.|NO MORE \/ �� ����� \d,\d{1,6}|HOLD)");
	tracingPattern.assign(LR"(E|H|N|T|NO \/ ���)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{8,9})");
	technologicalEnvironmentPattern.assign(LR"([A-Za-z]{2,4})");
	testEnvironmentPattern.assign(LR"((PLANT )?AIR ?/ ?(����������� )?������|FW / ��������������� ����������|NO / ���|WATER / ����)");
	paintingSystemPattern.assign(LR"(A|B|C|T|NO \/ ���)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO \/ ���)");
	weldInspectionPattern.assign(LR"(\d{1,3}%?( ���)?|\d ������ ��������� ��������|NO \/ ���)");
	testPressurePattern.assign(LR"(\d(,|\.)\d{1,3}|NO \/ ���)");
	GOSTPipelineCategoryPattern.assign(LR"(\w{1,3}(-\w{1,2})?((,|\.)? [A-Za-z�-��-�]{1,2})?( \/ \w{1,3} \(\w{3} \d{3}\))?)");
	designTemperaturePattern.assign(LR"(-?\d{1,3} ?( ?\/ ?|\.\.\.?)?\+?(\d{1,3})?)");
	designPressurePattern.assign(LR"((FV|-?\d((,|\.)\d{1,9})?)?( ?\/ ?(\d(,|\.)\d{2}|FV)?)?|HYDR\. \/ �������\.)");
	cipherDocumentPattern.assign(LR"(GCC-NAG-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"((DN)? ?\d{2,4}(( \/ |,) ?(DN)? ?\d{2,4})?)");
	isolationPattern.assign(LR"(\d{2,3}( \d{2,3})?( \d{2,3})?( \d{2,3})?( \d{2,3})?|H|NO \/ ���)");
	categoryPipelinesTRCUPattern.assign(LR"(NO \/ ���|\d|[0-9A-Z() /�-�]{10,20})");
	schemeNumberPattern.assign(LR"(GCC-NAG-DDD-\d{5}-\d{2}-\d{4}-\w{2}-\w{3}-\d{5}(, ?\d{4,5}){0,2})");
	lineNumberPattern.assign(LR"(\d{5}-\w{2}-\d{4}\/\d{3}-\w{2,4}-\d{4}-[0-9A-Z]{8,9}-\d{2})");
	stressCalculationPattern.assign(LR"(NO \/ ���|YES \/ ��)");
	isometricDrawingPattern.assign(LR"(\d{3}-\w{2,4}-\d{4}([ /0-9-]{2,5})?)");
	fileNamePattern.assign(LR"(GCC-NAG-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+\.dwg)");
}

bool DrawingPageNAG::trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck)
{
	if (operatingTemperatureStr == L"Ambient" || operatingTemperatureStr == L"AMBIENT / ���.�����" || operatingTemperatureStr == L"AMBIENT/ ���. �����") {
		operatingTemperature = L"AMBIENT / ���.�����";
		return true;
	}
	if (operatingTemperatureStr == L"�� ���� +5" || operatingTemperatureStr == L"�� ���� 5") {
		operatingTemperature = L"�� ���� 5 / No lower than 5";
		return true;
	}
	return DrawingPage::trySetOperatingTemperature(operatingTemperatureStr, assertionCheck);
}

bool DrawingPageNAG::trySetOperatingPressure(const std::wstring& operatingPressureStr, bool assertionCheck)
{
	if (operatingPressureStr == L"���." || operatingPressureStr == L"ATM / ���" || operatingPressureStr == L"��� / ATM" || operatingPressureStr == L"Atm" || operatingPressureStr == L"���.") 
	{
		operatingPressure = L"ATM / ���.";
		return true;
	}
	if (operatingPressureStr == L"HYDROSTATIC/ ����������������" || operatingPressureStr == L"HYDR. / �������") {
		operatingPressure = L"HYDR. / �������.";
		return true;
	}
	if (operatingPressureStr.starts_with(L"�� ����� / No more")) {
		std::wstring newOperatingPressureStr = operatingPressureStr;
		newOperatingPressureStr.replace(0, 17, L"NO MORE / �� �����");
		operatingPressure = newOperatingPressureStr;
		return true;
	}
	return DrawingPage::trySetOperatingPressure(operatingPressureStr, assertionCheck);
}

bool DrawingPageNAG::trySetTracing(const std::wstring& tracingStr, bool assertionCheck)
{
	if (tracingStr == L"NO" || tracingStr == L"NO/���") {
		tracing = L"NO / ���";
		return true;
	}
	return DrawingPage::trySetTracing(tracingStr, assertionCheck);
}

bool DrawingPageNAG::trySetTestEnvironment(const std::wstring& testEnvironmentStr, bool assertionCheck)
{
	if (testEnvironmentStr == L"WATER/����") {
		testEnvironment = L"WATER / ����";
		return true;
	}
	return DrawingPage::trySetTestEnvironment(testEnvironmentStr, assertionCheck);
}

bool DrawingPageNAG::trySetPaintingSystem(const std::wstring& paintingSystemStr, bool assertionCheck)
{
	if (paintingSystemStr == L"�") {
		paintingSystem = L"C";
		return true;
	}
	return DrawingPage::trySetPaintingSystem(paintingSystemStr, assertionCheck);
}

bool DrawingPageNAG::trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, bool assertionCheck)
{
	if (postWeldingHeatTreatmentStr == L"NO/���") {
		postWeldingHeatTreatment = L"NO / ���";
		return true;
	}
	return DrawingPage::trySetPostWeldingHeatTreatment(postWeldingHeatTreatmentStr, assertionCheck);
}

bool DrawingPageNAG::trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck)
{
	if (designPressureStr == L"HYDROSTATIC/ ����������������") {
		designPressure = L"HYDR. / �������.";
		return true;
	}
	return DrawingPage::trySetDesignPressure(designPressureStr, assertionCheck);
}

bool DrawingPageNAG::trySetIsolation(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"NO" || sourceStr == L"NO/���") {
		isolation = L"NO / ���";
		return true;
	}
	return DrawingPage::trySetIsolation(sourceStr, assertionCheck);
}

bool DrawingPageNAG::trySetStressCalculation(const std::wstring& stressCalculationStr, bool assertionCheck)
{
	if (stressCalculationStr == L"NO/���") {
		stressCalculation = L"NO / ���";
		return true;
	}
	if (stressCalculationStr == L"YES/��" || stressCalculationStr == L"YES/ ��") {
		stressCalculation = L"YES / ��";
		return true;
	}
	return DrawingPage::trySetStressCalculation(stressCalculationStr, assertionCheck);
}
