#include "DrawingPageASP.h"

#include "BuildComponent.h"
#include "BuildComponentASP.h"

#include <memory>
#include <regex>
#include <string>

DrawingPageASP::DrawingPageASP(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	operatingTemperaturePattern.assign(LR"(-?\d+(\.\d+\/\d+)?|env\.\/окр\.ср\.)");
	operatingPressurePattern.assign(LR"(\d{1}(\.\d{1,2})?|0\.3EX-01|атм\.)");
	tracingPattern.assign(LR"(NO ?\/ ?НЕТ|N|E|электрообогрев|unset\/не уст.)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{9})");
	technologicalEnvironmentPattern.assign(LR"([A-Z0-9]{2,4})");
	testEnvironmentPattern.assign(LR"(WATER\/ВОДА|вода\/water)");
	paintingSystemPattern.assign(LR"(\w\d->?\w\d|NO \/ НЕТ|Система \w, прим\. п\.1|В1-В2)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO ?\/ ?НЕТ)");
	weldInspectionPattern.assign(LR"(\d{1,3}(,\d)?%|операционный|пооперационно)");
	testPressurePattern.assign(LR"(\d\.\d{1,3}|атм\.)");
	GOSTPipelineCategoryPattern.assign(LR"([A-ZА-Яa-zа-я]{2,3}|NO \/ НЕТ)");
	designTemperaturePattern.assign(LR"(-?\d{1,3}(\/\d{1,3})?)");
	designPressurePattern.assign(LR"(\d(\.\d{1,3})?)");
	cipherDocumentPattern.assign(LR"(GCC-ASP-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"(DN\d{2,4})");
	isolationPattern.assign(LR"(H|N|NO \/ НЕТ|YES \/ ДА)");
	categoryPipelinesTRCUPattern.assign(LR"(NO \/ НЕТ|гр\. ?2, ?кат\. ?1)");
	schemeNumberPattern.assign(LR"((\/\d{3}-)?GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2}-\w{3}-\d{5} ?)");
	lineNumberPattern.assign(LR"(\d{3}-\w{2,4}-\d{4}-(\d{2}-)?(\d{3}-)?[A-Z0-9]{9}(-\w)?(_new)?)");
	stressCalculationPattern.assign(LR"(NO ?\/ ?НЕТ|YES ?\/ ?ДА)");
	isometricDrawingPattern.assign(LR"(\d{3}-\w{2,4}-\d{4}([ /0-9-]{2,5})?)");
	fileNamePattern.assign(LR"(\d{5}-\d{2}-\d{4}_\d{3}-\w{2,4}-\d{4}-[A-Z0-9]{9}-\d{2}_Sht__?\d{1,2}.dwg|GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+\.dwg)");
}

bool DrawingPageASP::trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck)
{
	if (operatingTemperatureStr == L"окр.среды") {
		operatingTemperature = L"env./окр.ср.";
		return true;
	}
	return DrawingPage::trySetOperatingTemperature(operatingTemperatureStr, assertionCheck);
}

bool DrawingPageASP::trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck)
{
	if (designPressureStr == L"окр.среды") {
		designPressure = L"env./окр.ср.";
		return true;
	}
	return DrawingPage::trySetDesignPressure(designPressureStr, assertionCheck);
}

bool DrawingPageASP::trySetCategoryPipelinesTRCU(const std::wstring& categoryPipelinesTRCUStr, bool assertionCheck)
{
	if (categoryPipelinesTRCUStr == L"-/-") {
		return true;
	}
	return DrawingPage::trySetCategoryPipelinesTRCU(categoryPipelinesTRCUStr, assertionCheck);
}

bool DrawingPageASP::trySetCipherDocument(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"") {
		return false;
	}
	return DrawingPage::trySetCipherDocument(sourceStr, assertionCheck);
}

bool DrawingPageASP::trySetWeldInspection(const std::wstring& weldInspectionStr, bool assertionCheck)
{
	if (weldInspectionStr.ends_with(L"%%%")) {
		std::wstring newWeldInpectionStr = weldInspectionStr;
		newWeldInpectionStr = newWeldInpectionStr.erase(newWeldInpectionStr.size() - 2, newWeldInpectionStr.size() - 1);
		return DrawingPage::trySetWeldInspection(newWeldInpectionStr, assertionCheck);
	}
	return DrawingPage::trySetWeldInspection(weldInspectionStr, assertionCheck);
}

bool DrawingPageASP::trySetSchemeNumber(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr.size() != 0 && (sourceStr.starts_with(L' ') || sourceStr.ends_with(L' ')))
	{
		return DrawingPage::trySetSchemeNumber(truncate(sourceStr), assertionCheck);
	}
	return DrawingPage::trySetSchemeNumber(sourceStr, assertionCheck);
}

bool DrawingPageASP::trySetLineNumber(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr.size() != 0 && (sourceStr.starts_with(L' ') || sourceStr.ends_with(L' ')))
	{
		return DrawingPage::trySetLineNumber(truncate(sourceStr), assertionCheck);
	}
	return DrawingPage::trySetLineNumber(sourceStr, assertionCheck);
}

bool DrawingPageASP::trySetIsometricDrawing(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr.size() != 0 && (sourceStr.starts_with(L' ') || sourceStr.ends_with(L' ')))
	{
		return DrawingPage::trySetIsometricDrawing(truncate(sourceStr), assertionCheck);
	}
	return DrawingPage::trySetIsometricDrawing(sourceStr, assertionCheck);
}
