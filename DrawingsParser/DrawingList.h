#pragma once

#include <vector>
#include <string>
#include "BuildComponent.h"

class DrawingList
{
protected:
	std::vector<BuildComponent> components;
	int totalSheets = 0;
	int currentSheet = 0;
	std::wstring operatingTemperature;
	std::wstring operatingPressure;
	std::wstring tracing; // Спутниковый обогрев
	std::wstring pipelineClass;
	std::wstring technologicalEnvironment;
	std::wstring testEnvironment;
	std::wstring paintingSystem;
	std::wstring postWeldingHeatTreatment; // Послесвар. Термообраб.
	std::wstring weldInspection;
	std::wstring testPressure;
	std::wstring GOSTPipelineCategory;
	std::wstring designTemperature; // Рассчётная температура
	std::wstring designPressure; // Рассчётное давление
	std::wstring cipherDocument;
	std::wstring diameterPipeline;
	std::wstring isolation;
	std::wstring categoryPipelinesTRCU; // Категория трубопр. ТР ТС
	std::wstring schemeNumber;
	std::wstring lineNumber;
	std::wstring stressCalculation; // Расчёт напряжений
	std::wstring isometricDrawing;
	std::wstring fileName;

public:
	void addComponent(const BuildComponent& component);
	bool trySetTotalSheets(const std::wstring& totalSheetsStr);
	bool trySetCurrentSheet(const std::wstring& currentSheetStr);
	virtual bool trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck = true) = 0;
	virtual bool trySetOperatingPressure(const std::wstring& operatingPressureStr, bool assertionCheck = true) = 0;
	virtual bool trySetTracing(const std::wstring& tracingStr, bool assertionCheck = true) = 0;
	virtual bool trySetPipelineClass(const std::wstring& pipelineClassStr, bool assertionCheck = true) = 0;
	virtual bool trySetTechnologicalEnvironment(const std::wstring& technologicalEnvironmentStr, bool assertionCheck = true) = 0;
	virtual bool trySetTestEnvironment(const std::wstring& testEnvironmentStr, bool assertionCheck = true) = 0;
	virtual bool trySetPaintingSystem(const std::wstring& paintingSystemStr, bool assertionCheck = true) = 0;
	virtual bool trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, bool assertionCheck = true) = 0;
	virtual bool trySetWeldInspection(const std::wstring& weldInspectionStr, bool assertionCheck = true) = 0;
	virtual bool trySetTestPressure(const std::wstring& testPressureStr, bool assertionCheck = true) = 0;
	virtual bool trySetGOSTPipelineCategory(const std::wstring& GOSTPipelineCategoryStr, bool assertionCheck = true) = 0;
	virtual bool trySetDesignTemperature(const std::wstring& designTemperatureStr, bool assertionCheck = true) = 0;
	virtual bool trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck = true) = 0;
	virtual bool trySetCipherDocument(const std::wstring& cipherDocumentStr, bool assertionCheck = true) = 0;
	virtual bool trySetDiameterPipeline(const std::wstring& diameterPipelineStr, bool assertionCheck = true) = 0;
	virtual bool trySetIsolation(const std::wstring& isolationStr, bool assertionCheck = true) = 0;
	virtual bool trySetCategoryPipelinesTRCU(const std::wstring& categoryPipelinesTRCUStr, bool assertionCheck = true) = 0;
	virtual bool trySetSchemeNumber(const std::wstring& schemeNumberStr, bool assertionCheck = true) = 0;
	virtual bool trySetLineNumber(const std::wstring& lineNumberStr, bool assertionCheck = true) = 0;
	virtual bool trySetStressCalculation(const std::wstring& stressCalculationStr, bool assertionCheck = true) = 0;
	virtual bool trySetIsometricDrawing(const std::wstring& isometricDrawingStr, bool assertionCheck = true) = 0;
	virtual bool trySetFileName(const std::wstring& fileNameStr, bool assertionCheck = true) = 0;

	size_t getComponentsCount();
	const BuildComponent& getComponent(size_t index);
	int getTotalSheets();
	int getCurrentSheet();
	std::wstring getOperatingTemperature();
	std::wstring getOperatingPressure();
	std::wstring getTracing();
	std::wstring getPipelineClass();
	std::wstring getTechnologicalEnvironment();
	std::wstring getTestEnvironment();
	std::wstring getPaintingSystem();
	std::wstring getPostWeldingHeatTreatment();
	std::wstring getWeldInspection();
	std::wstring getTestPressure();
	std::wstring getGOSTPipelineCategory();
	std::wstring getDesignTemperature();
	std::wstring getDesignPressure();
	std::wstring getCipherDocument();
	std::wstring getDiameterPipeline();
	std::wstring getIsolation();
	std::wstring getCategoryPipelinesTRCU();
	std::wstring getSchemeNumber();
	std::wstring getLineNumber();
	std::wstring getStressCalculation();
	std::wstring getIsometricDrawing();
	std::wstring getFileName();
};

