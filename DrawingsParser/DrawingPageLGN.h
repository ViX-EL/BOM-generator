#pragma once

#include "DrawingPage.h"

class DrawingPageLGN : public DrawingPage
{
public:
	DrawingPageLGN();
	//bool tryAddComponent(const std::wstring& componentNumberStr) override;
	bool trySetDesignPressure(const std::wstring& sourceStr, bool assertionCheck = true) override;
	bool trySetFileName(const std::wstring& sourceStr, bool assertionCheck = true) override;
	bool trySetStressCalculation(const std::wstring& sourceStr, bool assertionCheck = true) override;
};

