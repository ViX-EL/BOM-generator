#pragma once

#ifndef DrawingPageNAG_h__
#define DrawingPageNAG_h__

#include "DrawingPage.h"
#include "ValuesCheker.h"

class DrawingPageNAG : public DrawingPage
{
public:
	explicit DrawingPageNAG(bool inputCheckOff = false);
	bool trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetOperatingPressure(const std::wstring& operatingPressureStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetTracing(const std::wstring& tracingStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetTestEnvironment(const std::wstring& testEnvironmentStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetPaintingSystem(const std::wstring& paintingSystemStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetDesignPressure(const std::wstring& designPressureStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetIsolation(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetStressCalculation(const std::wstring& stressCalculationStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
};

#endif // DrawingPageNAG_h__

