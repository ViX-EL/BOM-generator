#include "DrawingPageASP.h"
#include "StringUtilities.h"
#include "BuildComponent.h"
#include "BuildComponentASP.h"
#include "ValuesCheker.h"

#include <memory>
#include <regex>
#include <string>

DrawingPageASP::DrawingPageASP(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	operatingTemperaturePattern.assign(LR"(-?\d+(?:\.\d+)?(?:\/\d+)?(?: ?- ?\d{1,3})?|env\.\/окр\.ср\.|UNSET)");
	operatingPressurePattern.assign(LR"(-?\d{1,2}(\.\d{1,6})?(?:(?:\/\d{1,2}(?:\.\d{1,4})?)?|(?: ?- ?\d{1,2}(\.\d{1,4})?))?|0\.3EX-01|атм\.|ATM\. \/АТМ\.|UNSET)");
	tracingPattern.assign(LR"(NO ?\/ ?НЕТ|YES ?\/ ?ДА|[NTE]|электрообогрев|unset(?:\/не уст\.)?)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{9}|Ошибка!)");
	technologicalEnvironmentPattern.assign(LR"([A-Z0-9]{2,4}|Ошибка!)");
	testEnvironmentPattern.assign(LR"(WATER ?\/ ?ВОДА|вода ?\/ ?water|AIR ?\/ ?ВОЗДУХ|ОШИБКА!)");
	paintingSystemPattern.assign(LR"(\w\d->?\w\d|NO ?\/ ?НЕТ|Система \w, прим\. п\.1|В1-В2)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO ?\/ ?НЕТ)");
	weldInspectionPattern.assign(LR"(\d{1,3}(,\d)?%|операционный|пооперационно|unset)");
	testPressurePattern.assign(LR"(\d\.\d{1,3}|атм\.)");
	GOSTPipelineCategoryPattern.assign(LR"([A-ZА-Яa-zа-я]{2,5}|NO \/ НЕТ)");
	designTemperaturePattern.assign(LR"(-?\d{1,3}(\.\d{1,3})?( ?\/ ?\d{1,3})?)");
	designPressurePattern.assign(LR"(\d(\.\d{1,3})?|ATM\.\/ АТМ\.?|UNSET)");
	cipherDocumentPattern.assign(LR"(GCC-ASP-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"(DN\d{2,4}|DNunset)");
	isolationPattern.assign(LR"(NO ?\/ ?НЕТ|[HNR]|YES ?\/ ?ДА|unset)");
	categoryPipelinesTRCUPattern.assign(LR"(NO \/ НЕТ|(?:GR\.\/ГР|гр)\. ?(?:\d|UNSET), ?(?:кат\.|CAT\.\/КАТ) ?(?:[\d-]|UNSET))");
	schemeNumberPattern.assign(LR"((\/\d{3}-)?GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-\w{3}-\d{5} ?)");
	lineNumberPattern.assign(LR"(\d{3}-\w{2,4}-\d{4}(?:-\d{2}-)?(?:\d{3}-)?(?:\-?[A-Z0-9]{9})?(?:_\d{1,2}\.\d{1,2})?(?:-\w)?(?:_new)?|Ошибка!)");
	stressCalculationPattern.assign(LR"(NO ?\/ ?НЕТ|YES ?\/ ?ДА)");
	isometricDrawingPattern.assign(LR"(\d{3}-\w{2,4}-\d{4}([ /0-9-]{2,5})?)");
	fileNamePattern.assign(
		LR"(\d{5}-\d{2}-\d{4}_\d{3}-\w{2,4}-\d{4}-[A-Z0-9]{9}-\d{2}_Sht__?\d{1,2}.dwg|GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+(?:\(\d+\))?(?:\.dxf)?\.dwg)");
}

bool DrawingPageASP::trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, ValuesCheker::Type checkType)
{
	if (operatingTemperatureStr == L"окр.среды" || operatingTemperatureStr.contains(L"AMB")) {
		operatingTemperature = L"env./окр.ср.";
		return true;
	}
	return DrawingPage::trySetOperatingTemperature(operatingTemperatureStr, checkType);
}

bool DrawingPageASP::trySetDesignTemperature(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"- / -") {
		designTemperature = L"-";
		return true;
	}
	return DrawingPage::trySetDesignTemperature(sourceStr, checkType);
}

bool DrawingPageASP::trySetDesignPressure(const std::wstring& designPressureStr, ValuesCheker::Type checkType)
{
	if (designPressureStr == L"окр.среды") {
		designPressure = L"env./окр.ср.";
		return true;
	}
	return DrawingPage::trySetDesignPressure(designPressureStr, checkType);
}

bool DrawingPageASP::trySetCategoryPipelinesTRCU(const std::wstring& categoryPipelinesTRCUStr, ValuesCheker::Type checkType)
{
	if (categoryPipelinesTRCUStr == L"-/-") {
		return true;
	}
	return DrawingPage::trySetCategoryPipelinesTRCU(categoryPipelinesTRCUStr, checkType);
}

bool DrawingPageASP::trySetCipherDocument(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"") {
		return false;
	}
	return DrawingPage::trySetCipherDocument(sourceStr, checkType);
}

bool DrawingPageASP::trySetWeldInspection(const std::wstring& weldInspectionStr, ValuesCheker::Type checkType)
{
	if (weldInspectionStr.ends_with(L"%%%")) {
		std::wstring newWeldInpectionStr = weldInspectionStr;
		newWeldInpectionStr = newWeldInpectionStr.erase(newWeldInpectionStr.size() - 2, newWeldInpectionStr.size() - 1);
		return DrawingPage::trySetWeldInspection(newWeldInpectionStr, checkType);
	}
	return DrawingPage::trySetWeldInspection(weldInspectionStr, checkType);
}

bool DrawingPageASP::trySetSchemeNumber(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr.size() != 0 && (sourceStr.starts_with(L' ') || sourceStr.ends_with(L' ')))
	{
		return DrawingPage::trySetSchemeNumber(StringUtilities::truncate(sourceStr), checkType);
	}
	return DrawingPage::trySetSchemeNumber(sourceStr, checkType);
}

bool DrawingPageASP::trySetLineNumber(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr.size() != 0 && (sourceStr.starts_with(L' ') || sourceStr.ends_with(L' ')))
	{
		return DrawingPage::trySetLineNumber(StringUtilities::truncate(sourceStr), checkType);
	}
	return DrawingPage::trySetLineNumber(sourceStr, checkType);
}

bool DrawingPageASP::trySetIsometricDrawing(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr.size() != 0 && (sourceStr.starts_with(L' ') || sourceStr.ends_with(L' ')))
	{
		return DrawingPage::trySetIsometricDrawing(StringUtilities::truncate(sourceStr), checkType);
	}
	return DrawingPage::trySetIsometricDrawing(sourceStr, checkType);
}
