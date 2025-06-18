#pragma once

#ifndef BuildComponentASP_h__
#define BuildComponentASP_h__

#include "BuildComponent.h"
#include "ValuesCheker.h"

class BuildComponentASP : public BuildComponent
{
public:
	explicit BuildComponentASP(const std::wstring& positionNumberStr);
	bool trySetPositionCode(const std::wstring& positionCodeStr, ValuesCheker::Type checkType = ValuesCheker::getType()) override;
};

#endif // BuildComponentASP_h__

