#include "DrawingPage.h"

#include "BuildComponent.h"
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

int DrawingPage::getCurrentPage() const
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

const std::wregex& DrawingPage::getPagesPattern()
{
	return pagesPattern;
}

std::wstring DrawingPage::truncate(const std::wstring& sourceStr)
{
	size_t startIndex = 0;
	for (wchar_t currentChar : sourceStr)
	{
		if (currentChar == L' ') {
			startIndex++;
		}
		else {
			break;
		}
	}

	size_t endIndex = sourceStr.size();
	for (size_t i = sourceStr.size() - 1; i > 0; i--)
	{
		if (sourceStr[i] == L' ') {
			endIndex--;
		}
		else {
			break;
		}
	}
	return sourceStr.substr(startIndex, endIndex);
}

DrawingPage::DrawingPage(bool inputCheckOff)
{
	this->inputCheckOff = inputCheckOff;
	pagesPattern.assign(LR"(\d{1,2})");
}

bool DrawingPage::trySetPages(const std::wstring& currentPageStr, const std::wstring& totalPagesStr, bool assertionCheck)
{
	bool isMatch = std::regex_match(currentPageStr, pagesPattern);
	isMatch = isMatch && std::regex_match(totalPagesStr, pagesPattern);
	if (assertionCheck) {
		assert(isMatch && "Значение для текущего листа и числа листов должно быть числом не больше двух символов!");
	}
	if (isMatch)
	{
		currentPage = stoi(currentPageStr);
		totalPages = stoi(totalPagesStr);
		return true;
	}
	return false;
}

bool DrawingPage::trySetOperatingTemperature(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") { 
		return true; 
	}
	return trySetValue(sourceStr, operatingTemperature, operatingTemperaturePattern, assertionCheck, "Недопустимое значение для рабочей температуры!", inputCheckOff);
}

bool DrawingPage::trySetOperatingPressure(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	if (sourceStr == L"атм.") {
		operatingPressure = L"atm.";
		return true;
	}
	return trySetValue(sourceStr, operatingPressure, operatingPressurePattern, assertionCheck, "Недопустимое значение для рабочего давления!", inputCheckOff);
}

bool DrawingPage::trySetTracing(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, tracing, tracingPattern, assertionCheck, "Недопустимое значение для спутникового обогрева!", inputCheckOff);
}

bool DrawingPage::trySetPipelineClass(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, pipelineClass, pipelineClassPattern, assertionCheck, "Недопустимое значение для класса трубопровода!", inputCheckOff);
}

bool DrawingPage::trySetTechnologicalEnvironment(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, technologicalEnvironment, technologicalEnvironmentPattern, assertionCheck, "Недопустимое значение для технологической среды!", inputCheckOff);
}

bool DrawingPage::trySetTestEnvironment(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, testEnvironment, testEnvironmentPattern, assertionCheck, "Недопустимое значение для среды испытаний!", inputCheckOff);
}

bool DrawingPage::trySetPaintingSystem(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, paintingSystem, paintingSystemPattern, assertionCheck, "Недопустимое значение для системы покраски!", inputCheckOff);
}

bool DrawingPage::trySetPostWeldingHeatTreatment(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, postWeldingHeatTreatment, postWeldingHeatTreatmentPattern, assertionCheck, "Недопустимое значение для послесварочной термообработки!", inputCheckOff);
}

bool DrawingPage::trySetWeldInspection(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, weldInspection, weldInspectionPattern, assertionCheck, "Недопустимое значение для контроля сварных швов!", inputCheckOff);
}

bool DrawingPage::trySetTestPressure(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, testPressure, testPressurePattern, assertionCheck, "Недопустимое значение для давления испытаний!", inputCheckOff);
}

bool DrawingPage::trySetGOSTPipelineCategory(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, GOSTPipelineCategory, GOSTPipelineCategoryPattern, assertionCheck, "Недопустимое значение для категории трубопровода Гост!", inputCheckOff);
}

bool DrawingPage::trySetDesignTemperature(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, designTemperature, designTemperaturePattern, assertionCheck, "Недопустимое значение для рассчётной температуры!", inputCheckOff);
}

bool DrawingPage::trySetDesignPressure(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, designPressure, designPressurePattern, assertionCheck, "Недопустимое значение для рассчётного давления!", inputCheckOff);
}

bool DrawingPage::trySetCipherDocument(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, cipherDocument, cipherDocumentPattern, assertionCheck, "Недопустимое значение для шифра документа!", inputCheckOff);
}

bool DrawingPage::trySetDiameterPipeline(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, diameterPipeline, diameterPipelinePattern, assertionCheck, "Недопустимое значение для диаметра трубопровода!", inputCheckOff);
}

bool DrawingPage::trySetIsolation(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, isolation, isolationPattern, assertionCheck, "Недопустимое значение для изоляции!", inputCheckOff);
}

bool DrawingPage::trySetCategoryPipelinesTRCU(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, categoryPipelinesTRCU, categoryPipelinesTRCUPattern, assertionCheck, "Недопустимое значение категории трубопровода ТР ТС!", inputCheckOff);
}

bool DrawingPage::trySetSchemeNumber(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-" || sourceStr == L"No/Нет") {
		return true;
	}
	return trySetValue(sourceStr, schemeNumber, schemeNumberPattern, assertionCheck, "Недопустимое значение для номера схемы!", inputCheckOff);
}

bool DrawingPage::trySetLineNumber(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, lineNumber, lineNumberPattern, assertionCheck, "Недопустимое значение для номера линии!", inputCheckOff);
}

bool DrawingPage::trySetStressCalculation(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, stressCalculation, stressCalculationPattern, assertionCheck, "Недопустимое значение для расчёта напряжений!", inputCheckOff);
}

bool DrawingPage::trySetIsometricDrawing(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, isometricDrawing, isometricDrawingPattern, assertionCheck, "Недопустимое значение для изометрического чертежа!", inputCheckOff);
}

bool DrawingPage::trySetFileName(const std::wstring& sourceStr, bool assertionCheck)
{
	if (sourceStr == L"-") {
		return true;
	}
	return trySetValue(sourceStr, fileName, fileNamePattern, assertionCheck, "Недопустимое значение для имени файла!", inputCheckOff);
}
