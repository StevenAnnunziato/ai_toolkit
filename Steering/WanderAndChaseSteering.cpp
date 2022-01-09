#include <cassert>

#include "Steering.h"
#include "WanderAndChaseSteering.h"
#include "SeekSteering.h"
#include "FaceSteering.h"
#include "WanderSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


WanderAndChaseSteering::WanderAndChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::ARRIVE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	mpWanderSteering = new WanderSteering(ownerID);
}
WanderAndChaseSteering::~WanderAndChaseSteering()
{
	delete mpWanderSteering;
	mpWanderSteering = nullptr;
}

Steering* WanderAndChaseSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	// set up variables for behavior
	float thresholdDist = 200;

	// get distance from target
	Vector2D dir = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	float dist = dir.getLength();

	// close enough to seek
	if (dist < thresholdDist)
	{
		// set up seek steering
		SeekSteering seek(mOwnerID, mTargetLoc, mTargetID);
		seek.update();
		data = seek.getData();

		// set up face steering
		FaceSteering face(mOwnerID, mTargetLoc);
		face.update();
		PhysicsData faceData = face.getData();
		data.rotAcc = faceData.rotAcc;
		data.rotVel = faceData.rotVel;

		//std::cout << "seeking, dist: " << dist << std::endl;
	}
	else // far enough - wander
	{
		// set up wander steering
		mpWanderSteering->update();
		data = mpWanderSteering->getData();

		//std::cout << "wandering" << std::endl;
	}

	this->mData = data;
	return this;
}

