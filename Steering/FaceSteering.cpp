#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include <cmath>
#include <iostream>

FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc)
	:Steering(Steering::FACE,ownerID,targetLoc)
{
}

FaceSteering::~FaceSteering()
{
}

Steering* FaceSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	Vector2D diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	double targetFacing = diff.calcFacing();
	double currentFacing = pOwner->getFacing();

	double rotDiff = calcFacingDiff(currentFacing,targetFacing);
	double absRotDiff = abs(rotDiff);

	double slowThreshold = 1.0;
	double stopThreshold = .01;

	//std::cout << absRotDiff << std::endl;
	
	if (absRotDiff > slowThreshold)//accelerate as fast as possible
	{
		data.rotAcc = pOwner->getMaxRotAcc();
		//get sign correctly
		if (!isTargetToRight())
			data.rotAcc = -data.rotAcc;
	}
	else if(absRotDiff < stopThreshold)//should we stop?
	{
		data.rotAcc = 0;
		data.rotVel = 0;
	}
	else //we should be slowing
	{
		data.rotAcc = 0;
		data.rotVel = (float)((absRotDiff / slowThreshold) * pOwner->getMaxRotVel());
		//get sign correctly
		if (!isTargetToRight())
			data.rotVel = -data.rotVel;
	}

	this->mData = data;
	return this;
}

double FaceSteering::calcFacingDiff(double facing, double targetFacing)
{
	double rotDiff = mapToRangeMinusPiToPi(facing - targetFacing);
	return rotDiff;
}

bool FaceSteering::isTargetToRight()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	Vector2D headingVector = pOwner->getHeadingVector();
	Vector2D right(-headingVector.getY(), headingVector.getX());

	Vector2D diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	float dot = diff.dotProduct(right);
	//std::cout << dot << " " << headingVector << " " << right << " " << diff << std::endl;
	if (dot >= 0.0f)
		return true;
	else
		return false;
}

