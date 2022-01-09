#include <cassert>

#include "Steering.h"
#include "CohesionSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


CohesionSteering::CohesionSteering(const UnitID& ownerID)
	: Steering()
{
	mType = Steering::COHESION;
	setOwnerID(ownerID);
}

Steering* CohesionSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	// get nearby units
	int dist = gpGame->getUnitManager()->getFlockData()->cohesionDist;
	std::vector<Unit*> nearbyUnits = gpGame->getUnitManager()->getUnitsInNeighborhood(pOwner, (float)dist);

	// get average position of these units
	Vector2D avg;
	if (nearbyUnits.size() > 0)
	{
		std::vector<Unit*>::iterator it = nearbyUnits.begin();
		while (it != nearbyUnits.end())
		{
			avg += (*it)->getPositionComponent()->getPosition();

			it++;
		}
		avg /= (float)nearbyUnits.size();
	}
	else
	{
		avg = pOwner->getPositionComponent()->getPosition();
	}

	// apply a force towards this point
	Vector2D awayVector = avg - pOwner->getPositionComponent()->getPosition();

	// apply force to this unit
	// note: this could be done as an acceleration but I chose velocity instead
	data.vel = awayVector;

	this->mData = data;
	return this;
}
