#pragma once

#include "DrawingList.h"

#include <string>

class DrawingListASP : public DrawingList
{
	bool trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck = true) override;
	bool trySetOperatingPressure(const std::wstring& operatingPressureStr, bool assertionCheck = true) override;
	bool trySetTracing(const std::wstring& tracingStr, bool assertionCheck = true) override;
	bool trySetPipelineClass(const std::wstring& pipelineClassStr, bool assertionCheck = true) override;
	bool trySetTechnologicalEnvironment(const std::wstring& technologicalEnvironmentStr, bool assertionCheck = true) override;
	bool trySetTestEnvironment(const std::wstring& testEnvironmentStr, bool assertionCheck = true) override;
	bool trySetPaintingSystem(const std::wstring& paintingSystemStr, bool assertionCheck = true) override;
	bool trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, bool assertionCheck = true) override;
	bool trySetWeldInspection(const std::wstring& weldInspectionStr, bool assertionCheck = true) override;
	bool trySetTestPressure(const std::wstring& testPressureStr, bool assertionCheck = true) override;
	bool trySetGOSTPipelineCategory(const std::wstring& GOSTPipelineCategoryStr, bool assertionCheck = true) override;
	bool trySetDesignTemperature(const std::wstring& designTemperatureStr, bool assertionCheck = true) override;
	bool trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck = true) override;
	bool trySetCipherDocument(const std::wstring& cipherDocumentStr, bool assertionCheck = true) override;
	bool trySetDiameterPipeline(const std::wstring& diameterPipelineStr, bool assertionCheck = true) override;
	bool trySetIsolation(const std::wstring& isolationStr, bool assertionCheck = true) override;
	bool trySetCategoryPipelinesTRCU(const std::wstring& categoryPipelinesTRCUStr, bool assertionCheck = true) override;
	bool trySetSchemeNumber(const std::wstring& schemeNumberStr, bool assertionCheck = true) override;
	bool trySetLineNumber(const std::wstring& lineNumberStr, bool assertionCheck = true) override;
	bool trySetStressCalculation(const std::wstring& stressCalculationStr, bool assertionCheck = true) override;
	bool trySetIsometricDrawing(const std::wstring& isometricDrawingStr, bool assertionCheck = true) override;
	bool trySetFileName(const std::wstring& fileNameStr, bool assertionCheck = true) override;
};

