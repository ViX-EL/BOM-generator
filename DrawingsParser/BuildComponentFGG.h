#pragma once

#ifndef BuildComponentFGG_h__
#define BuildComponentFGG_h__

#include "BuildComponent.h"
#include "ValuesCheker.h"

class BuildComponentFGG : public BuildComponent
{
public:
	explicit BuildComponentFGG(const std::wstring& positionNumberStr);
};

#endif // BuildComponentFGG_h__

