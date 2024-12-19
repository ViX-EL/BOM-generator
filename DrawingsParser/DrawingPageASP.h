#pragma once

#include "DrawingPage.h"

#include <string>

class BuildComponentASP;

class DrawingPageASP : public DrawingPage
{
public:
	DrawingPageASP();
	bool trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck = true) override;
	bool trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck = true) override;
	bool trySetCategoryPipelinesTRCU(const std::wstring& categoryPipelinesTRCUStr, bool assertionCheck = true) override;
	bool trySetCipherDocument(const std::wstring& sourceStr, bool assertionCheck = true) override;
	bool trySetWeldInspection(const std::wstring& weldInspectionStr, bool assertionCheck = true) override;
	bool trySetSchemeNumber(const std::wstring& sourceStr, bool assertionCheck = true) override;
	bool trySetLineNumber(const std::wstring& sourceStr, bool assertionCheck = true) override;
	bool trySetIsometricDrawing(const std::wstring& sourceStr, bool assertionCheck = true) override;
};

