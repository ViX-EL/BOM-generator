﻿#pragma once

#ifndef DrawingPage_h__
#define DrawingPage_h__

#include "BuildComponent.h"
#include "ValuesCheker.h"

#include <vector>
#include <string>
#include <memory>
#include <cassert>

class DrawingPage
{
protected:
	std::vector<std::shared_ptr<BuildComponent>> components;
	int totalPages = 0;
	std::wstring currentPage = L"-";
	bool inputCheckOff = false;
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

	std::wregex totalPagesPattern;
	std::wregex currentPagePattern;
	std::wregex operatingTemperaturePattern;
	std::wregex operatingPressurePattern;
	std::wregex tracingPattern;
	std::wregex pipelineClassPattern;
	std::wregex technologicalEnvironmentPattern;
	std::wregex testEnvironmentPattern;
	std::wregex paintingSystemPattern;
	std::wregex postWeldingHeatTreatmentPattern;
	std::wregex weldInspectionPattern;
	std::wregex testPressurePattern;
	std::wregex GOSTPipelineCategoryPattern;
	std::wregex designTemperaturePattern;
	std::wregex designPressurePattern;
	std::wregex cipherDocumentPattern;
	std::wregex diameterPipelinePattern;
	std::wregex isolationPattern;
	std::wregex categoryPipelinesTRCUPattern;
	std::wregex schemeNumberPattern;
	std::wregex lineNumberPattern;
	std::wregex stressCalculationPattern;
	std::wregex isometricDrawingPattern;
	std::wregex fileNamePattern;

	explicit DrawingPage(bool inputCheckOff = false);
public:
	template <typename T>
	bool tryAddComponent(const std::wstring& componentNumberStr);
	template <typename T>
	bool tryAddComponent(int componentNumber);
	bool trySetPages(const std::wstring& currentPageStr, const std::wstring& totalPagesStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetOperatingTemperature(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetOperatingPressure(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetTracing(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	bool trySetPipelineClass(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	bool trySetTechnologicalEnvironment(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetTestEnvironment(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetPaintingSystem(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetPostWeldingHeatTreatment(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetWeldInspection(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	bool trySetTestPressure(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	bool trySetGOSTPipelineCategory(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetDesignTemperature(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetDesignPressure(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetCipherDocument(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	bool trySetDiameterPipeline(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetIsolation(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetCategoryPipelinesTRCU(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetSchemeNumber(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetLineNumber(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetStressCalculation(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetIsometricDrawing(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());
	virtual bool trySetFileName(const std::wstring& sourceStr,  ValuesCheker::Type checkType = ValuesCheker::getType());

	size_t getComponentsCount();
	BuildComponent& getComponent(size_t index);
	BuildComponent& getLastComponent();
	int getTotalPages() const;
	std::wstring getCurrentPage() const;

	std::wstring getOperatingTemperature() const;
	std::wstring getOperatingPressure() const;
	std::wstring getTracing() const;
	std::wstring getPipelineClass() const;
	std::wstring getTechnologicalEnvironment() const;
	std::wstring getTestEnvironment() const;
	std::wstring getPaintingSystem() const;
	std::wstring getPostWeldingHeatTreatment() const;
	std::wstring getWeldInspection() const;
	std::wstring getTestPressure() const;
	std::wstring getGOSTPipelineCategory() const;
	std::wstring getDesignTemperature() const;
	std::wstring getDesignPressure() const;
	std::wstring getCipherDocument() const;
	std::wstring getDiameterPipeline() const;
	std::wstring getIsolation() const;
	std::wstring getCategoryPipelinesTRCU() const;
	std::wstring getSchemeNumber() const;
	std::wstring getLineNumber() const;
	std::wstring getStressCalculation() const;
	std::wstring getIsometricDrawing() const;
	std::wstring getFileName() const;

	const std::wregex& getOperatingTemperaturePattern();
	const std::wregex& getOperatingPressurePattern();
	const std::wregex& getTracingPattern();
	const std::wregex& getPipelineClassPattern();
	const std::wregex& getTechnologicalEnvironmentPattern();
	const std::wregex& getTestEnvironmentPattern();
	const std::wregex& getPaintingSystemPattern();
	const std::wregex& getPostWeldingHeatTreatmentPattern();
	const std::wregex& getWeldInspectionPattern();
	const std::wregex& getTestPressurePattern();
	const std::wregex& getGOSTPipelineCategoryPattern();
	const std::wregex& getDesignTemperaturePattern();
	const std::wregex& getDesignPressurePattern();
	const std::wregex& getCipherDocumentPattern();
	const std::wregex& getDiameterPipelinePattern();
	const std::wregex& getIsolationPattern();
	const std::wregex& getCategoryPipelinesTRCUPattern();
	const std::wregex& getSchemeNumberPattern();
	const std::wregex& getLineNumberPattern();
	const std::wregex& getStressCalculationPattern();
	const std::wregex& getIsometricDrawingPattern();
	const std::wregex& getFileNamePattern();
	const std::wregex& getTotalPagesPattern();
	const std::wregex& getCurrentPagePattern();

	void parseSplitComponentsData();
};

template<typename T>
inline bool DrawingPage::tryAddComponent(const std::wstring& componentNumberStr)
{
	if (std::regex_match(componentNumberStr, BuildComponent::getPositionNumberPattern()))
	{
		components.emplace_back(std::make_shared<T>(componentNumberStr));
		return true;
	}
	return false;
}

template<typename T>
inline bool DrawingPage::tryAddComponent(int componentNumber)
{
	assert(componentNumber > 0 && "Номер компонента должен быть больше 0!");
	components.emplace_back(std::make_shared<T>(componentNumber));
	return true;
}

#endif // DrawingPage_h__
