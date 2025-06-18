#pragma once

#ifndef BuildComponentSEC_h__
#define BuildComponentSEC_h__

#include "BuildComponent.h"

class BuildComponentSEC : public BuildComponent
{
public:
	explicit BuildComponentSEC(const std::wstring& positionNumberStr);
};

#endif // BuildComponentSEC_h__
