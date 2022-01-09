#pragma once

#include "Steering.h"
#include <Vector2D.h>

class FaceSteering :public Steering
{
public:
	FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc);
	~FaceSteering();

	double static calcFacingDiff(double facing, double targetFacing);
	bool isTargetToRight();

protected:
	Steering* getSteering();
};
