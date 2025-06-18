#pragma once

#ifndef BuildComponentIOT_h__
#define BuildComponentIOT_h__

#include "BuildComponent.h"

class BuildComponentIOT : public BuildComponent
{
public:
	explicit BuildComponentIOT(const std::wstring& positionNumberStr);
	explicit BuildComponentIOT(int positionNumber);
private:
	void initPatterns();
};

#endif // BuildComponentIOT_h__
