#pragma once

#ifndef DrawingPageLGN_h__
#define DrawingPageLGN_h__

#include "DrawingPage.h"
#include "ValuesCheker.h"

class DrawingPageLGN : public DrawingPage
{
public:
	explicit DrawingPageLGN(bool inputCheckOff);
	bool trySetDesignPressure(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetFileName(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetWeldInspection(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetStressCalculation(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
	bool trySetOperatingTemperature(const std::wstring& sourceStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
};

#endif // DrawingPageLGN_h__
