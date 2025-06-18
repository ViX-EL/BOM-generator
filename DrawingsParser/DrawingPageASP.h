#pragma once

#ifndef DrawingPageASP_h__
#define DrawingPageASP_h__

#include "DrawingPage.h"
#include "ValuesCheker.h"

#include <string>

class BuildComponentASP;

class DrawingPageASP : public DrawingPage
{
public:
	explicit DrawingPageASP(bool inputCheckOff = false);
	bool trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetDesignTemperature(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetDesignPressure(const std::wstring& designPressureStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetCategoryPipelinesTRCU(const std::wstring& categoryPipelinesTRCUStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetCipherDocument(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetWeldInspection(const std::wstring& weldInspectionStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetSchemeNumber(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetLineNumber(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetIsometricDrawing(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
};

#endif // DrawingPageASP_h__
