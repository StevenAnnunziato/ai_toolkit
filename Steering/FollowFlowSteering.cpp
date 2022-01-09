#include <cassert>

#include "Steering.h"
#include "FollowFlowSteering.h"
#include "Game.h"
#include "Unit.h"
#include "UnitManager.h"
#include "FlowField.h"
#include "Grid.h"


FollowFlowSteering::FollowFlowSteering(const UnitID& ownerID)
	: Steering()
{
	setOwnerID(ownerID);
}

Steering* FollowFlowSteering::getSteering()
{
	// get physics data
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	// find the desired velocity direction
	Vector2D pos = pOwner->getPositionComponent()->getPosition();
	int cellIndex = gpGame->getGrid()->getCellIndexFromPixelXY((int)pos.getX(), (int)pos.getY());

	// set velocity to this direction
	data.vel = *gpGame->getFlowField()->getVectorField()->get(cellIndex);
	data.vel = data.vel * data.maxSpeed;

	this->mData = data;
	return this;
}