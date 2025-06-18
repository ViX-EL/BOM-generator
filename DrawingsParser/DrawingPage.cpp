#include "DrawingPage.h"
#include "BuildComponent.h"
#include "ValuesCheker.h"

#include <regex>
#include <cassert>
#include <vector>
#include <string>

size_t DrawingPage::getComponentsCount()
{
	return components.size();
}

BuildComponent& DrawingPage::getComponent(size_t index)
{
	assert(index < components.size() && "Невозможно получить компонент с несуществующим индексом!");
	return *components.at(index);
}

BuildComponent& DrawingPage::getLastComponent()
{
	assert(components.size() != 0 && "Невозможно получить компонент с несуществующим индексом!");
	return *(components.back());
}

int DrawingPage::getTotalPages() const
{
	return totalPages;
}

std::wstring DrawingPage::getCurrentPage() const
{
	return currentPage;
}

std::wstring DrawingPage::getOperatingTemperature() const
{
	return operatingTemperature != L"" ? operatingTemperature : L"-";
}

std::wstring DrawingPage::getOperatingPressure() const
{
	return operatingPressure != L"" ? operatingPressure : L"-";
}

std::wstring DrawingPage::getTracing() const
{
	return tracing != L"" ? tracing : L"-";
}

std::wstring DrawingPage::getPipelineClass() const
{
	return pipelineClass != L"" ? pipelineClass : L"-";
}

std::wstring DrawingPage::getTechnologicalEnvironment() const
{
	return technologicalEnvironment != L"" ? technologicalEnvironment : L"-";
}

std::wstring DrawingPage::getTestEnvironment() const
{
	return testEnvironment != L"" ? testEnvironment : L"-";
}

std::wstring DrawingPage::getPaintingSystem() const
{
	return paintingSystem != L"" ? paintingSystem : L"-";
}

std::wstring DrawingPage::getPostWeldingHeatTreatment() const
{
	return postWeldingHeatTreatment != L"" ? postWeldingHeatTreatment : L"-";
}

std::wstring DrawingPage::getWeldInspection() const
{
	return weldInspection != L"" ? weldInspection : L"-";
}

std::wstring DrawingPage::getTestPressure() const
{
	return testPressure != L"" ? testPressure : L"-";
}

std::wstring DrawingPage::getGOSTPipelineCategory() const
{
	return GOSTPipelineCategory != L"" ? GOSTPipelineCategory : L"-";
}

std::wstring DrawingPage::getDesignTemperature() const
{
	return designTemperature != L"" ? designTemperature : L"-";
}

std::wstring DrawingPage::getDesignPressure() const
{
	return designPressure != L"" ? designPressure : L"-";
}

std::wstring DrawingPage::getCipherDocument() const
{
	return cipherDocument != L"" ? cipherDocument : L"-";
}

std::wstring DrawingPage::getDiameterPipeline() const
{
	return diameterPipeline != L"" ? diameterPipeline : L"-";
}

std::wstring DrawingPage::getIsolation() const
{
	return isolation != L"" ? isolation : L"-";
}

std::wstring DrawingPage::getCategoryPipelinesTRCU() const
{
	return categoryPipelinesTRCU != L"" ? categoryPipelinesTRCU : L"-";
}

std::wstring DrawingPage::getSchemeNumber() const
{
	return schemeNumber != L"" ? schemeNumber : L"-";
}

std::wstring DrawingPage::getLineNumber() const
{
	return lineNumber != L"" ? lineNumber : L"-";
}

std::wstring DrawingPage::getStressCalculation() const
{
	return stressCalculation != L"" ? stressCalculation : L"-";
}

std::wstring DrawingPage::getIsometricDrawing() const
{
	return isometricDrawing != L"" ? isometricDrawing : L"-";
}

std::wstring DrawingPage::getFileName() const
{
	return fileName != L"" ? fileName : L"-";
}

const std::wregex& DrawingPage::getOperatingTemperaturePattern()
{
	return operatingTemperaturePattern;
}

const std::wregex& DrawingPage::getOperatingPressurePattern()
{
	return operatingPressurePattern;
}

const std::wregex& DrawingPage::getTracingPattern()
{
	return tracingPattern;
}

const std::wregex& DrawingPage::getPipelineClassPattern()
{
	return pipelineClassPattern;
}

const std::wregex& DrawingPage::getTechnologicalEnvironmentPattern()
{
	return technologicalEnvironmentPattern;
}

const std::wregex& DrawingPage::getTestEnvironmentPattern()
{
	return testEnvironmentPattern;
}

const std::wregex& DrawingPage::getPaintingSystemPattern()
{
	return paintingSystemPattern;
}

const std::wregex& DrawingPage::getPostWeldingHeatTreatmentPattern()
{
	return postWeldingHeatTreatmentPattern;
}

const std::wregex& DrawingPage::getWeldInspectionPattern()
{
	return weldInspectionPattern;
}

const std::wregex& DrawingPage::getTestPressurePattern()
{
	return testPressurePattern;
}

const std::wregex& DrawingPage::getGOSTPipelineCategoryPattern()
{
	return GOSTPipelineCategoryPattern;
}

const std::wregex& DrawingPage::getDesignTemperaturePattern()
{
	return designTemperaturePattern;
}

const std::wregex& DrawingPage::getDesignPressurePattern()
{
	return designPressurePattern;
}

const std::wregex& DrawingPage::getCipherDocumentPattern()
{
	return cipherDocumentPattern;
}

const std::wregex& DrawingPage::getDiameterPipelinePattern()
{
	return diameterPipelinePattern;
}

const std::wregex& DrawingPage::getIsolationPattern()
{
	return isolationPattern;
}

const std::wregex& DrawingPage::getCategoryPipelinesTRCUPattern()
{
	return  categoryPipelinesTRCUPattern;
}

const std::wregex& DrawingPage::getSchemeNumberPattern()
{
	return schemeNumberPattern;
}

const std::wregex& DrawingPage::getLineNumberPattern()
{
	return lineNumberPattern;
}

const std::wregex& DrawingPage::getStressCalculationPattern()
{
	return stressCalculationPattern;
}

const std::wregex& DrawingPage::getIsometricDrawingPattern()
{
	return isometricDrawingPattern;
}

const std::wregex& DrawingPage::getFileNamePattern()
{
	return fileNamePattern;
}

const std::wregex& DrawingPage::getTotalPagesPattern()
{
	return totalPagesPattern;
}

const std::wregex& DrawingPage::getCurrentPagePattern()
{
	return currentPagePattern;
}

void DrawingPage::parseSplitComponentsData()
{
	for (size_t i = 0; i < components.size(); i++) {
		components[i]->parseSplitData();
	}
}

DrawingPage::DrawingPage(bool inputCheckOff)
{
	this->inputCheckOff = inputCheckOff;
	totalPagesPattern.assign(LR"(\d{1,2})");
	currentPagePattern = totalPagesPattern;
}

bool DrawingPage::trySetPages(const std::wstring& currentPageStr, const std::wstring& totalPagesStr, ValuesCheker::Type checkType)
{
	bool isMatch = ValuesCheker::trySetValue(currentPageStr, currentPage, currentPagePattern, false , checkType, "Недопустимое значение для текущего листа!");
	isMatch = isMatch && ValuesCheker::trySetValue(totalPagesStr, totalPages, totalPagesPattern, checkType, "Недопустимое значение для числа листов!");
	return isMatch;
}

bool DrawingPage::trySetOperatingTemperature(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") { 
		return true; 
	}
	return ValuesCheker::trySetValue(sourceStr, operatingTemperature, operatingTemperaturePattern, inputCheckOff, checkType, "Недопустимое значение для рабочей температуры!");
}

bool DrawingPage::trySetOperatingPressure(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	if (sourceStr == L"атм.") {
		operatingPressure = L"atm.";
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, operatingPressure, operatingPressurePattern, inputCheckOff, checkType, "Недопустимое значение для рабочего давления!");
}

bool DrawingPage::trySetTracing(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, tracing, tracingPattern, inputCheckOff, checkType, "Недопустимое значение для спутникового обогрева!");
}

bool DrawingPage::trySetPipelineClass(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, pipelineClass, pipelineClassPattern, inputCheckOff, checkType, "Недопустимое значение для класса трубопровода!");
}

bool DrawingPage::trySetTechnologicalEnvironment(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, technologicalEnvironment, technologicalEnvironmentPattern, inputCheckOff, checkType, "Недопустимое значение для технологической среды!");
}

bool DrawingPage::trySetTestEnvironment(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, testEnvironment, testEnvironmentPattern, inputCheckOff, checkType, "Недопустимое значение для среды испытаний!");
}

bool DrawingPage::trySetPaintingSystem(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, paintingSystem, paintingSystemPattern, inputCheckOff, checkType, "Недопустимое значение для системы покраски!");
}

bool DrawingPage::trySetPostWeldingHeatTreatment(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, postWeldingHeatTreatment, postWeldingHeatTreatmentPattern, inputCheckOff, checkType, "Недопустимое значение для послесварочной термообработки!");
}

bool DrawingPage::trySetWeldInspection(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, weldInspection, weldInspectionPattern, inputCheckOff, checkType, "Недопустимое значение для контроля сварных швов!");
}

bool DrawingPage::trySetTestPressure(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, testPressure, testPressurePattern, inputCheckOff, checkType, "Недопустимое значение для давления испытаний!");
}

bool DrawingPage::trySetGOSTPipelineCategory(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, GOSTPipelineCategory, GOSTPipelineCategoryPattern, inputCheckOff, checkType, "Недопустимое значение для категории трубопровода Гост!");
}

bool DrawingPage::trySetDesignTemperature(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, designTemperature, designTemperaturePattern, inputCheckOff, checkType, "Недопустимое значение для рассчётной температуры!");
}

bool DrawingPage::trySetDesignPressure(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, designPressure, designPressurePattern, inputCheckOff, checkType, "Недопустимое значение для рассчётного давления!");
}

bool DrawingPage::trySetCipherDocument(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, cipherDocument, cipherDocumentPattern, inputCheckOff, checkType, "Недопустимое значение для шифра документа!");
}

bool DrawingPage::trySetDiameterPipeline(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, diameterPipeline, diameterPipelinePattern, inputCheckOff, checkType, "Недопустимое значение для диаметра трубопровода!");
}

bool DrawingPage::trySetIsolation(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, isolation, isolationPattern, inputCheckOff, checkType, "Недопустимое значение для изоляции!");
}

bool DrawingPage::trySetCategoryPipelinesTRCU(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, categoryPipelinesTRCU, categoryPipelinesTRCUPattern, inputCheckOff, checkType, "Недопустимое значение категории трубопровода ТР ТС!");
}

bool DrawingPage::trySetSchemeNumber(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-" || sourceStr == L"No/Нет") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, schemeNumber, schemeNumberPattern, inputCheckOff, checkType, "Недопустимое значение для номера схемы!");
}

bool DrawingPage::trySetLineNumber(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, lineNumber, lineNumberPattern, inputCheckOff, checkType, "Недопустимое значение для номера линии!");
}

bool DrawingPage::trySetStressCalculation(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, stressCalculation, stressCalculationPattern, inputCheckOff, checkType, "Недопустимое значение для расчёта напряжений!");
}

bool DrawingPage::trySetIsometricDrawing(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, isometricDrawing, isometricDrawingPattern, inputCheckOff, checkType, "Недопустимое значение для изометрического чертежа!");
}

bool DrawingPage::trySetFileName(const std::wstring& sourceStr, ValuesCheker::Type checkType)
{
	if (sourceStr == L"-") {
		return true;
	}
	return ValuesCheker::trySetValue(sourceStr, fileName, fileNamePattern, inputCheckOff, checkType, "Недопустимое значение для имени файла!");
}
