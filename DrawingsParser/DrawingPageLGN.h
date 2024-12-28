#pragma once

#include "DrawingPage.h"

class DrawingPageLGN : public DrawingPage
{
public:
	explicit DrawingPageLGN(bool inputCheckOff);
	bool trySetDesignPressure(const std::wstring& sourceStr, bool assertionCheck = true) override;
	bool trySetFileName(const std::wstring& sourceStr, bool assertionCheck = true) override;
	bool trySetWeldInspection(const std::wstring& sourceStr, bool assertionCheck = true) override;
	bool trySetStressCalculation(const std::wstring& sourceStr, bool assertionCheck = true) override;
};

