#include <cassert>

#include "Steering.h"
#include "WanderSteering.h"
#include "SeekSteering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Vector2D.h"

WanderSteering::WanderSteering(const UnitID& ownerID)
	: Steering()
{
	mType = Steering::WANDER;
	setOwnerID(ownerID);
	mWanderOrientation = 0;
}

Steering* WanderSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	PositionData posData = pOwner->getPositionComponent()->getData();

	// set up variables for behavior
	float wanderOffset = 200;
	float wanderRadius = 90;
	float wanderRate = 0.5;

	// update wander orientation
	mWanderOrientation += genRandomBinomial() * wanderRate;

	// calculate the combined target orientation
	float targetOrientation = mWanderOrientation + posData.facing;

	// calculate the center of the wander circle
	mTargetLoc = posData.pos + (pOwner->getHeadingVector() * wanderOffset);

	// calculate target location
	mTargetLoc += Vector2D::getVectorInDirection(targetOrientation) * wanderRadius;
	//std::cout << Vector2D::getVectorInDirection(targetOrientation) * wanderRadius << std::endl;

	// set up face steering
	FaceSteering face(mOwnerID, mTargetLoc);
	face.update();
	PhysicsData faceData = face.getData();
	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;

	// accellerate to full
	Vector2D dir = (mTargetLoc - posData.pos);
	data.acc = dir.getNormalizedVector() * pOwner->getMaxAcc();
	
	this->mData = data;
	return this;
}

