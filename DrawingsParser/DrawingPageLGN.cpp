#include "DrawingPageLGN.h"
#include "BuildComponentLGN.h"

#include <string>
#include <regex>

DrawingPageLGN::DrawingPageLGN(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	operatingTemperaturePattern.assign(LR"(-?\d{1,3}(\.\d)?( \/ \d{1,3}(\.\d)?)?|to\/�� \d{1,2})");
	operatingPressurePattern.assign(LR"((\d{1,2}| +)?((\.\d{1,3})? ?\/ \d{1,2}(\.\d{1,3})?)?|(hydr\. \/ )?(to\/�� )?\d{1,2}\.\d{1,3}|hydr\.|atm\.)");
	tracingPattern.assign(LR"(E|N|T)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{9,11})");
	technologicalEnvironmentPattern.assign(LR"([A-Z0-9]{1,4}|No\/���)");
	testEnvironmentPattern.assign(LR"(Water/����|������\/���� Air\/Nitrogen)");
	paintingSystemPattern.assign(LR"(A|B|C|F)");
	postWeldingHeatTreatmentPattern.assign(LR"(No\/���|��)");
	weldInspectionPattern.assign(LR"(\d{1,3}%?%?(\d{3})?( \(LPT\))?(, \d{1,3}%)?|No/���)");
	testPressurePattern.assign(LR"(\d{1,2}(\.\d{1,2})?)");
	GOSTPipelineCategoryPattern.assign(LR"([A-Za-z�-��-�(), ]+)");
	designTemperaturePattern.assign(LR"(-?\d{2,3} \/ \d{2,3}|to\/�� \d{2})");
	designPressurePattern.assign(LR"(-?\d{1,2}(\.\d{1,4})?( \/ (FV)?(\d\.\d{2})?)?|atm\.|hydr\.|to\/�� \d)");
	cipherDocumentPattern.assign(LR"(GCC-LGN-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"(DN\d{2,4}(mm)?)");
	isolationPattern.assign(LR"(C|H|M|N|P|R)");
	categoryPipelinesTRCUPattern.assign(LR"(\d|No\/���)");
	schemeNumberPattern.assign(LR"((GCC-LGN-DDD-\d{5}-\d{2}-\d{4}-\w{2,3}-PID-(\d{5},?)+;?)+)");
	lineNumberPattern.assign(LR"(\d{3}-[A-Z0-9]{1,4}-\d{4}-[A-Z0-9]{9,11}|Protective duct drain\/���� ��������� ������ �\d{2}(_|-)(\d{2,3}|\w{1,3})(_|-)\d{2})");
	stressCalculationPattern.assign(LR"(NO\/���|YES\/��)");
	isometricDrawingPattern.assign(LR"(\d{3}-[A-Z0-9]{1,4}-\d{4}-[A-Z0-9]{9,11}|Protective duct drain\/���� ��������� ������ �\d{2}(_|-)(\d{2,3}|\w{1,3})(_|-)\d{2})");
	fileNamePattern.assign(LR"(GCC-LGN-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+(\.dwg|\.dxf))");
}

bool DrawingPageLGN::trySetDesignPressure(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"���.") {
		designPressure = L"atm.";
		return true;
	}
	return DrawingPage::trySetDesignPressure(sourceStr, assertionCheck);
}

bool DrawingPageLGN::trySetFileName(const std::wstring& sourceStr, bool assertionCheck)
{
	std::wstring toDeleteStr = L"��� ����� ";
	std::wstring newFileNameStr = sourceStr;
	newFileNameStr = newFileNameStr.erase(0, toDeleteStr.size());
	return DrawingPage::trySetFileName(newFileNameStr, assertionCheck);
}

bool DrawingPageLGN::trySetWeldInspection(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr.ends_with(L"%%%")) {
		std::wstring newWeldInpectionStr = sourceStr;
		newWeldInpectionStr = newWeldInpectionStr.erase(newWeldInpectionStr.size() - 2, newWeldInpectionStr.size() - 1);
		return DrawingPage::trySetWeldInspection(newWeldInpectionStr, assertionCheck);
	}
	return DrawingPage::trySetWeldInspection(sourceStr, assertionCheck);
}

bool DrawingPageLGN::trySetStressCalculation(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"Yes/��")
	{
		stressCalculation = L"YES/��";
		return true;
	}
	if (sourceStr == L"No/���") 
	{
		stressCalculation = L"NO/���";
		return true;
	}
	return DrawingPage::trySetStressCalculation(sourceStr, assertionCheck);
}
