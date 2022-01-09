#include <cassert>

#include "Steering.h"
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::ARRIVE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* ArriveSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	// define some variables for behavior
	float stoppingDistance = 200;
	float arriveDistance = 20;
	float timeToTarget = 2.3f;
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	// get direction
	Vector2D dir = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	// get distance from target
	float dist = dir.getLength();
	// NOTE: this could be optimized by using square distances but I'd rather optimize later

	// arrived at target
	if (dist < arriveDistance)
		return nullptr;

	// inside radius - slow down
	float targetSpeed;
	if (dist < stoppingDistance)
	{
		// manually interpolate speed between maxSpeed and 0
		targetSpeed = pOwner->getMaxSpeed() * dist / stoppingDistance;
	}
	// outside radius - speed up
	else
	{
		targetSpeed = pOwner->getMaxSpeed();
	}

	// combine speed and direction into target velocity
	Vector2D targetVelocity = dir.getNormalizedVector() * targetSpeed;

	// come to a stop if needed
	if (dist < stoppingDistance)
	{
		data.acc = targetVelocity - data.vel * timeToTarget;
	}
	else {
		data.acc = targetVelocity;
	}
	
	// limit acceleration
	if (data.acc.getLength() > pOwner->getMaxAcc())
	{
		data.acc.normalize();
		data.acc *= pOwner->getMaxAcc();
	}

	this->mData = data;
	return this;
}

