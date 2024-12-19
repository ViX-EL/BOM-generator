#include "DrawingPageNAG.h"

DrawingPageNAG::DrawingPageNAG()
{
	operatingTemperaturePattern.assign(LR"((-|\+)?\d{2,3}(\.\.\.\+?\d{2,3})?|AMBIENT \/ ���\. �����|�� ���� \d \/ No lower than \d)");
	operatingPressurePattern.assign(LR"(-?\d,\d{1,6}(\.\.\.\d,\d{1,6})?|ATM \/ ���\.|HYDR\. \/ �������|NO MORE \/ �� ����� \d,\d{1,6}|HOLD)");
	tracingPattern.assign(LR"(E|H|N|T|NO \/ ���)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{8,9})");
	technologicalEnvironmentPattern.assign(LR"([A-Z]{2,4})");
	testEnvironmentPattern.assign(LR"((PLANT )?AIR ?/ ?(����������� )?������|FW / ��������������� ����������|NO / ���|WATER / ����)");
	paintingSystemPattern.assign(LR"(A|B|C|T|NO \/ ���)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO \/ ���)");
	weldInspectionPattern.assign(LR"(\d{1,3}%?( ���)?|\d ������ ��������� ��������|NO \/ ���)");
	testPressurePattern.assign(LR"(\d,\d{1,3}|NO \/ ���)");
	GOSTPipelineCategoryPattern.assign(LR"(\w{1,3}(-\w{1,2})?((,|\.) \w{1,2})?( \/ \w{1,3} \(\w{3} \d{3}\))?)"); //[A-Za-z�-��-� .,/()0-9-]{4,22}
	designTemperaturePattern.assign(LR"(-?\d{1,3} ?( ?\/ ?|\.\.\.?)?\+?(\d{1,3})?)");
	designPressurePattern.assign(LR"((FV|-?\d(,\d{1,9})?)?( ?\/ ?(\d(,|\.)\d{2}|FV)?)?|HYDR\. \/ �������\.)");
	cipherDocumentPattern.assign(LR"(GCC-NAG-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"(DN ?\d{2,4}(( \/ |,) ?(DN)? ?\d{2,4})?)");
	isolationPattern.assign(LR"(\d{2,3}( \d{2,3})?( \d{2,3})?( \d{2,3})?( \d{2,3})?|H|NO \/ ���)");
	categoryPipelinesTRCUPattern.assign(LR"(NO \/ ���|\d|[0-9A-Z() /�-�]{10,20})");
	schemeNumberPattern.assign(LR"(GCC-NAG-DDD-\d{5}-\d{2}-\d{4}-\w{2}-\w{3}-\d{5}(, ?\d{5}){0,2})");
	lineNumberPattern.assign(LR"(\d{5}-\w{2}-\d{4}\/\d{3}-\w{2,4}-\d{4}-[0-9A-Z]{9}-\d{2})");
	stressCalculationPattern.assign(LR"(NO \/ ���|YES \/ ��)");
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
	if (operatingTemperatureStr == L"Ambient" || operatingTemperatureStr == L"AMBIENT / ���.�����" || operatingTemperatureStr == L"AMBIENT/ ���. �����") {
		newOperatingTemperatureStr = L"AMBIENT / ���.�����";
	}
	if (operatingTemperatureStr == L"�� ���� +5" || operatingTemperatureStr == L"�� ���� 5") {
		newOperatingTemperatureStr = L"�� ���� 5 / No lower than 5";
	}

	if (newOperatingTemperatureStr != L"") {
		return DrawingPage::trySetOperatingTemperature(newOperatingTemperatureStr, assertionCheck);
	}
	return DrawingPage::trySetOperatingTemperature(operatingTemperatureStr, assertionCheck);
}

bool DrawingPageNAG::trySetOperatingPressure(const std::wstring& operatingPressureStr, bool assertionCheck)
{
	std::wstring newOperatingPressureStr;
	if (newOperatingPressureStr == L"���." || newOperatingPressureStr == L"ATM / ���" || newOperatingPressureStr == L"Atm") {
		newOperatingPressureStr = L"ATM / ���.";
	}
	if (newOperatingPressureStr == L"HYDROSTATIC/ ����������������") {
		newOperatingPressureStr = L"HYDR. / �������";
	}
	if (newOperatingPressureStr.starts_with(L"�� ����� / No more")) {
		newOperatingPressureStr.replace(0, 17, L"NO MORE / �� �����");
	}
	if (newOperatingPressureStr != L"") {
		return DrawingPage::trySetOperatingPressure(newOperatingPressureStr, assertionCheck);
	}
	return DrawingPage::trySetOperatingPressure(operatingPressureStr, assertionCheck);
}

bool DrawingPageNAG::trySetTracing(const std::wstring& tracingStr, bool assertionCheck)
{
	if (tracingStr == L"NO" || tracingStr == L"NO/���") {
		return DrawingPage::trySetOperatingPressure(L"NO / ���", assertionCheck);
	}
	return DrawingPage::trySetTracing(tracingStr, assertionCheck);
}

bool DrawingPageNAG::trySetTestEnvironment(const std::wstring& testEnvironmentStr, bool assertionCheck)
{
	if (testEnvironmentStr == L"WATER/����") {
		return DrawingPage::trySetOperatingPressure(L"WATER / ����", assertionCheck);
	}
	return DrawingPage::trySetTestEnvironment(testEnvironmentStr, assertionCheck);
}

bool DrawingPageNAG::trySetPaintingSystem(const std::wstring& paintingSystemStr, bool assertionCheck)
{
	if (paintingSystemStr == L"�") {
		return DrawingPage::trySetPaintingSystem(L"C", assertionCheck);
	}
	return DrawingPage::trySetPaintingSystem(paintingSystemStr, assertionCheck);
}

bool DrawingPageNAG::trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, bool assertionCheck)
{
	if (postWeldingHeatTreatmentStr == L"NO/���") {
		DrawingPage::trySetPostWeldingHeatTreatment(L"NO / ���", assertionCheck);
	}
	return DrawingPage::trySetPostWeldingHeatTreatment(postWeldingHeatTreatmentStr, assertionCheck);
}

bool DrawingPageNAG::trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck)
{
	if (designPressureStr == L"HYDROSTATIC/ ����������������") {
		DrawingPage::trySetDesignPressure(L"HYDR. / �������.", assertionCheck);
	}
	return DrawingPage::trySetDesignPressure(designPressureStr, assertionCheck);
}

bool DrawingPageNAG::trySetStressCalculation(const std::wstring& stressCalculationStr, bool assertionCheck)
{
	std::wstring newStressCalculationStr;
	if (newStressCalculationStr == L"NO/���") {
		newStressCalculationStr = L"NO / ���";
	}
	if (newStressCalculationStr == L"YES/��" || newStressCalculationStr == L"YES/ ��") {
		newStressCalculationStr = L"YES / ��";
	}
	if (newStressCalculationStr != L"") {
		return DrawingPage::trySetStressCalculation(newStressCalculationStr, assertionCheck);
	}
	return DrawingPage::trySetStressCalculation(stressCalculationStr, assertionCheck);
}
