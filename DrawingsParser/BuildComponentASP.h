#pragma once

#include "BuildComponent.h"

class BuildComponentASP : public BuildComponent
{
public:
	BuildComponentASP(const std::wstring& positionNumberStr);
	bool trySetPositionCode(const std::wstring& positionCodeStr, bool assertionCheck = true) override;
private:
	std::wstring replaceRuSimbols(const std::wstring& sourceStr);
};

