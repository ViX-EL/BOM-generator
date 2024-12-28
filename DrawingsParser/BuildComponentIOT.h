#pragma once

#include "BuildComponent.h"

class BuildComponentIOT : public BuildComponent
{
public:
	explicit BuildComponentIOT(const std::wstring& positionNumberStr);
	explicit BuildComponentIOT(int positionNumber);
private:
	void initPatterns();
};

