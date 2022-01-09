#include <cassert>

#include "Steering.h"
#include "AlignmentSteering.h"
#include "FaceSteering.h"
#include "SeekSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


AlignmentSteering::AlignmentSteering(const UnitID& ownerID)
	: Steering()
{
	mType = Steering::GROUP_ALIGNMENT;
	setOwnerID(ownerID);
}

Steering* AlignmentSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData physData = pOwner->getPhysicsComponent()->getData();
	PositionData posData = pOwner->getPositionComponent()->getData();

	// get nearby units
	int dist = gpGame->getUnitManager()->getFlockData()->alignmentDist;
	std::vector<Unit*> nearbyUnits = gpGame->getUnitManager()->getUnitsInNeighborhood(pOwner, (float)dist, true);

	// get an average heading vector from these units
	Vector2D avg;
	if (nearbyUnits.size() > 0)
	{
		std::vector<Unit*>::iterator it = nearbyUnits.begin();
		while (it != nearbyUnits.end())
		{
			avg += (*it)->getHeadingVector();

			it++;
		}
		avg /= (float)nearbyUnits.size();
	}
	else
	{
		// make sure one lone unit will move in the correct direction
		avg = pOwner->getHeadingVector();
	}

	//physData.vel = avg.getNormalizedVector() * pOwner->getMaxSpeed();

	// lerp towards the target velocity
	float lerpSpeed = 0.6f;
	physData.vel = pOwner->getHeadingVector() + (avg.getNormalizedVector() - pOwner->getHeadingVector()) * lerpSpeed;

	// steer towards this average velocity
	// take the average velocity and convert it to a point ahead of the unit's forward direction
	//avg += posData.pos;

	// set up face steering
	/*FaceSteering face(mOwnerID, avg);
	face.update();
	PhysicsData faceData = face.getData();
	physData.rotAcc = faceData.rotAcc;
	physData.rotVel = faceData.rotVel;*/

	// move forward
	/*Vector2D myTarget = posData.pos + pOwner->getHeadingVector();
	SeekSteering seek(mOwnerID, myTarget, INVALID_UNIT_ID);
	seek.update();
	physData.acc = seek.getData().acc;*/

	this->mData = physData;
	return this;
}
