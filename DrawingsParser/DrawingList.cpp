#include "DrawingList.h"

#include "BuildComponent.h"
#include <regex>
#include <cassert>
#include <vector>
#include <string>

void DrawingList::addComponent(const BuildComponent& component)
{
	components.push_back(component);
}

size_t DrawingList::getComponentsCount()
{
	return components.size();
}

const BuildComponent& DrawingList::getComponent(size_t index)
{
	assert(index < components.size() && "Невозможно получить компонент с несуществующим индексом!");
	return components.at(index);
}

int DrawingList::getTotalSheets()
{
	return totalSheets;
}

int DrawingList::getCurrentSheet()
{
	return currentSheet;
}

std::wstring DrawingList::getOperatingTemperature()
{
	return operatingTemperature != L"" ? operatingTemperature : L"-";
}

std::wstring DrawingList::getOperatingPressure()
{
	return operatingPressure != L"" ? operatingPressure : L"-";
}

std::wstring DrawingList::getTracing()
{
	return tracing != L"" ? tracing : L"-";
}

std::wstring DrawingList::getPipelineClass()
{
	return pipelineClass != L"" ? pipelineClass : L"-";
}

std::wstring DrawingList::getTechnologicalEnvironment()
{
	return technologicalEnvironment != L"" ? technologicalEnvironment : L"-";
}

std::wstring DrawingList::getTestEnvironment()
{
	return testEnvironment != L"" ? testEnvironment : L"-";
}

std::wstring DrawingList::getPaintingSystem()
{
	return paintingSystem != L"" ? paintingSystem : L"-";
}

std::wstring DrawingList::getPostWeldingHeatTreatment()
{
	return postWeldingHeatTreatment != L"" ? postWeldingHeatTreatment : L"-";
}

std::wstring DrawingList::getWeldInspection()
{
	return weldInspection != L"" ? weldInspection : L"-";
}

std::wstring DrawingList::getTestPressure()
{
	return testPressure != L"" ? testPressure : L"-";
}

std::wstring DrawingList::getGOSTPipelineCategory()
{
	return GOSTPipelineCategory != L"" ? GOSTPipelineCategory : L"-";
}

std::wstring DrawingList::getDesignTemperature()
{
	return designTemperature != L"" ? designTemperature : L"-";
}

std::wstring DrawingList::getDesignPressure()
{
	return designPressure != L"" ? designPressure : L"-";
}

std::wstring DrawingList::getCipherDocument()
{
	return cipherDocument != L"" ? cipherDocument : L"-";
}

std::wstring DrawingList::getDiameterPipeline()
{
	return diameterPipeline != L"" ? diameterPipeline : L"-";
}

std::wstring DrawingList::getIsolation()
{
	return isolation != L"" ? isolation : L"-";
}

std::wstring DrawingList::getCategoryPipelinesTRCU()
{
	return categoryPipelinesTRCU != L"" ? categoryPipelinesTRCU : L"-";
}

std::wstring DrawingList::getSchemeNumber()
{
	return schemeNumber != L"" ? schemeNumber : L"-";
}

std::wstring DrawingList::getLineNumber()
{
	return lineNumber != L"" ? lineNumber : L"-";
}

std::wstring DrawingList::getStressCalculation()
{
	return stressCalculation != L"" ? stressCalculation : L"-";
}

std::wstring DrawingList::getIsometricDrawing()
{
	return isometricDrawing != L"" ? isometricDrawing : L"-";
}

std::wstring DrawingList::getFileName()
{
	return fileName != L"" ? fileName : L"-";
}

bool DrawingList::trySetTotalSheets(const std::wstring& totalSheetsStr)
{
	std::wregex listsPattern(LR"(\d{1,2})");
	bool isMatch = std::regex_match(totalSheetsStr, listsPattern);
	assert(isMatch && "Значение для числа листов должно быть числом не больше двух символов!");
	if (isMatch)
	{
		totalSheets = stoi(totalSheetsStr);
		return true;
	}
	return false;
}

bool DrawingList::trySetCurrentSheet(const std::wstring& currentSheetStr)
{
	std::wregex listsPattern(LR"(\d{1,2})");
	bool isMatch = std::regex_match(currentSheetStr, listsPattern);
	assert(isMatch && "Значение для текущего листа должно быть числом не больше двух символов!");
	if (isMatch)
	{
		totalSheets = stoi(currentSheetStr);
		return true;
	}
	return false;
}
