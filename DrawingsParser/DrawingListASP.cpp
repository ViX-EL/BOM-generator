#include "DrawingListASP.h"

bool DrawingListASP::trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck)
{
	if (operatingTemperatureStr == L"���.�����") {
		operatingTemperature = L"env./���.��.";
	}
	return trySetValue(operatingTemperatureStr, operatingTemperature, std::wregex(LR"(-?\d+(\.\d+\/\d+)?|env\.\/���\.��\.)"), assertionCheck,
		"������������ �������� ��� ������� �����������!");
}

bool DrawingListASP::trySetOperatingPressure(const std::wstring& operatingPressureStr, bool assertionCheck)
{
	return trySetValue(operatingPressureStr, operatingPressure, std::wregex(LR"(\d{1}(\.\d{1,2})?|0\.3EX-01|���\.)"), assertionCheck, "������������ �������� ��� �������� ��������!");
}

bool DrawingListASP::trySetTracing(const std::wstring& tracingStr, bool assertionCheck)
{
	if (tracingStr == L"unset/�� ���.") {
		return true;
	}
	return trySetValue(tracingStr, tracing, std::wregex(LR"(NO ?\/ ?���|N|E|��������������)"), assertionCheck, "������������ �������� ��� ������������ ��������!");
}

bool DrawingListASP::trySetPipelineClass(const std::wstring& pipelineClassStr, bool assertionCheck)
{
	return trySetValue(pipelineClassStr, pipelineClass, std::wregex(LR"([A-Z0-9]{9})"), assertionCheck, "������������ �������� ��� ������ ������������!");
}

bool DrawingListASP::trySetTechnologicalEnvironment(const std::wstring& technologicalEnvironmentStr, bool assertionCheck)
{
	return trySetValue(technologicalEnvironmentStr, technologicalEnvironment, std::wregex(LR"([A-Z0-9]{2,4})"), assertionCheck, "������������ �������� ��� ��������������� �����!");
}

bool DrawingListASP::trySetTestEnvironment(const std::wstring& testEnvironmentStr, bool assertionCheck)
{
	return trySetValue(testEnvironmentStr, testEnvironment, std::wregex(LR"(WATER/����|����/water)"), assertionCheck, "������������ �������� ��� ����� ���������!");
}

bool DrawingListASP::trySetPaintingSystem(const std::wstring& paintingSystemStr, bool assertionCheck)
{
	return trySetValue(paintingSystemStr, paintingSystem, std::wregex(LR"(\w\d->?\w\d|NO \/ ���|������� \w, ����\. �\.1|�1-�2)"), assertionCheck,
		"������������ �������� ��� ������� ��������!");
}

bool DrawingListASP::trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, bool assertionCheck)
{
	return trySetValue(postWeldingHeatTreatmentStr, postWeldingHeatTreatment, std::wregex(LR"(NO ?\/ ?���)"), assertionCheck,
		"������������ �������� ��� �������������� ��������������!");
}

bool DrawingListASP::trySetWeldInspection(const std::wstring& weldInspectionStr, bool assertionCheck)
{
	return trySetValue(weldInspectionStr, weldInspection, std::wregex(LR"(\d(,\d)?%|������������|�������������)"), assertionCheck,
		"������������ �������� ��� �������� ������� ����!");
}

bool DrawingListASP::trySetTestPressure(const std::wstring& testPressureStr, bool assertionCheck)
{
	return trySetValue(testPressureStr, testPressure, std::wregex(LR"(\d\.\d{1,3}|���\.)"), assertionCheck, "������������ �������� ��� �������� ���������!");
}

bool DrawingListASP::trySetGOSTPipelineCategory(const std::wstring& GOSTPipelineCategoryStr, bool assertionCheck)
{
	return trySetValue(GOSTPipelineCategoryStr, GOSTPipelineCategory, std::wregex(LR"([A-Z]{2,3}|NO \/ ���)"), assertionCheck, "������������ �������� ��� ��������� ������������ ����!");
}

bool DrawingListASP::trySetDesignTemperature(const std::wstring& designTemperatureStr, bool assertionCheck)
{
	return trySetValue(designTemperatureStr, designTemperature, std::wregex(LR"(-?\d{1,3}(\/\d{1,3})?)"), assertionCheck, "������������ �������� ��� ���������� �����������!");
}

bool DrawingListASP::trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck)
{
	if (designPressureStr == L"���.�����") {
		designPressure = L"env./���.��.";
		return true;
	}
	return trySetValue(designPressureStr, designPressure, std::wregex(LR"(\d(\.\d{1,3})?)"), assertionCheck, "������������ �������� ��� ����������� �������!");
}

bool DrawingListASP::trySetCipherDocument(const std::wstring& cipherDocumentStr, bool assertionCheck)
{
	return trySetValue(cipherDocumentStr, cipherDocument, std::wregex(LR"(GCC-ASP-DDD-\d+-\d+-\d+-\w+-ISO-\d+)"), assertionCheck, "������������ �������� ��� ����� ���������!");
}

bool DrawingListASP::trySetDiameterPipeline(const std::wstring& diameterPipelineStr, bool assertionCheck)
{
	return trySetValue(diameterPipelineStr, diameterPipeline, std::wregex(LR"(DN\d{2,4})"), assertionCheck, "������������ �������� ��� �������� ������������!");
}

bool DrawingListASP::trySetIsolation(const std::wstring& isolationStr, bool assertionCheck)
{
	return trySetValue(isolationStr, isolation, std::wregex(LR"(H|N|NO \/ ���|YES \/ ��)"), assertionCheck, "������������ �������� ��� ��������!");
}

bool DrawingListASP::trySetCategoryPipelinesTRCU(const std::wstring& categoryPipelinesTRCUStr, bool assertionCheck)
{
	if (categoryPipelinesTRCUStr == L"-/-") {
		return true;
	}
	return trySetValue(categoryPipelinesTRCUStr, categoryPipelinesTRCU, std::wregex(LR"(NO \/ ���|��\. ?2, ?���\. ?1)"), assertionCheck, 
		"������������ �������� ��������� ������������ �� ��!");
}

bool DrawingListASP::trySetSchemeNumber(const std::wstring& schemeNumberStr, bool assertionCheck)
{
	return trySetValue(schemeNumberStr, schemeNumber, std::wregex(LR"((\/\d{3}-)?GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2}-\w{3}-\d{5})"), assertionCheck,
		"������������ �������� ��� ������ �����!");
}

bool DrawingListASP::trySetLineNumber(const std::wstring& lineNumberStr, bool assertionCheck)
{
	return trySetValue(lineNumberStr, lineNumber, std::wregex(LR"(\d{3}-\w{2,4}-\d{4}-[A-Z0-9]{9}(_new)?)"), assertionCheck, "������������ �������� ��� ������ �����!");
}

bool DrawingListASP::trySetStressCalculation(const std::wstring& stressCalculationStr, bool assertionCheck)
{
	return trySetValue(stressCalculationStr, stressCalculation, std::wregex(LR"(NO ?\/ ?���|YES ?\/ ?��)"), assertionCheck, "������������ �������� ��� ������� ����������!");
}

bool DrawingListASP::trySetIsometricDrawing(const std::wstring& isometricDrawingStr, bool assertionCheck)
{
	return trySetValue(isometricDrawingStr, isometricDrawing, std::wregex(LR"(\d{3}-\w{2,4}-\d{4}([ /0-9-]{2,5})?)"), assertionCheck,
		"������������ �������� ��� ��������������� �������!");
}

bool DrawingListASP::trySetFileName(const std::wstring& fileNameStr, bool assertionCheck)
{
	return trySetValue(fileNameStr, fileName,
		std::wregex(LR"(\d{5}-\d{2}-\d{4}_\d{3}-\w{2}-\d{4}-[A-Z0-9]{9}-\d{2}_Sht_\d{1,2}.dwg|GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+\.dwg)"), assertionCheck,
		"������������ �������� ��� ����� �����!");
}
