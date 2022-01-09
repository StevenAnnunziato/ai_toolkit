#include <cassert>

#include "Steering.h"
#include "SeparationSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


SeparationSteering::SeparationSteering(const UnitID& ownerID)
	: Steering()
{
	mType = Steering::SEPARATION;
	setOwnerID(ownerID);
}

Steering* SeparationSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData physData = pOwner->getPhysicsComponent()->getData();
	//PositionData posData = pOwner->getPositionComponent()->getData();

	// get nearby units
	int dist = gpGame->getUnitManager()->getFlockData()->separationDist;
	std::vector<Unit*> nearbyUnits = gpGame->getUnitManager()->getUnitsInNeighborhood(pOwner, (float)dist);

	// combine forces away from other units
	Vector2D myPos = pOwner->getPositionComponent()->getPosition();
	Vector2D repulsiveForce;
	std::vector<Unit*>::iterator it = nearbyUnits.begin();
	while (it != nearbyUnits.end())
	{
		// get vector
		Vector2D diff = myPos - (*it)->getPositionComponent()->getPosition();
		float dist = diff.getLengthSquared(); // normalize
		// combine with 1/r^2 weighting
		repulsiveForce += diff.getNormalizedVector() * (1.0f / dist);

		it++;
	}

	// apply the force to this unit
	// NOTE: Applying this as an acceleration would also work.
	physData.vel = repulsiveForce * 10000; // TODO: get rid of this magic number

	this->mData = physData;
	return this;
}
