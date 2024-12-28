#pragma once

#include "DrawingPage.h"

class DrawingPageNAG : public DrawingPage
{
public:
	explicit DrawingPageNAG(bool inputCheckOff = false);
	bool trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck = true) override;
	bool trySetOperatingPressure(const std::wstring& operatingPressureStr, bool assertionCheck = true) override;
	bool trySetTracing(const std::wstring& tracingStr, bool assertionCheck = true) override;
	bool trySetTestEnvironment(const std::wstring& testEnvironmentStr, bool assertionCheck = true) override;
	bool trySetPaintingSystem(const std::wstring& paintingSystemStr, bool assertionCheck = true) override;
	bool trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, bool assertionCheck = true) override;
	bool trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck = true) override;
	bool trySetIsolation(const std::wstring& sourceStr, bool assertionCheck = true) override;
	bool trySetStressCalculation(const std::wstring& stressCalculationStr, bool assertionCheck = true) override;
};

