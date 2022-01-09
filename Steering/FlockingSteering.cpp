#include <cassert>

#include "Steering.h"
#include "FlockingSteering.h"
#include "CohesionSteering.h"
#include "AlignmentSteering.h"
#include "SeparationSteering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


FlockingSteering::FlockingSteering(const UnitID& ownerID)
	: Steering()
{
	mType = Steering::FLOCKING;
	setOwnerID(ownerID);
}

Steering* FlockingSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	// set up cohesion steering
	CohesionSteering cohesion(mOwnerID);
	cohesion.update();
	Vector2D cohesionVector = cohesion.getData().vel.getNormalizedVector();

	// set up separation steering
	SeparationSteering separation(mOwnerID);
	separation.update();
	Vector2D separationVector = separation.getData().vel.getNormalizedVector();

	// set up alignment steering
	AlignmentSteering alignment(mOwnerID);
	alignment.update();
	Vector2D alignmentVector = alignment.getData().vel.getNormalizedVector();

	// combine these three steering vectors into one flocking vector
	std::vector<Vector2D> inputs;
	inputs.push_back(cohesionVector);
	inputs.push_back(separationVector);
	inputs.push_back(alignmentVector);
	// get weights
	FlockData* pFlockData = gpGame->getUnitManager()->getFlockData();
	std::vector<float> weights;
	weights.push_back(pFlockData->cohesionWeight);
	weights.push_back(pFlockData->separationWeight);
	weights.push_back(pFlockData->alignmentWeight);
	Vector2D flockingVector = weightedBlend(inputs, weights);

	// apply flocking vector to this unit's data
	float speedModifier = 0.6f;
	data.vel = flockingVector.getNormalizedVector() * pOwner->getMaxSpeed() * speedModifier;
	
	// look where this unit is going
	Vector2D lookPos = pOwner->getPositionComponent()->getPosition() + data.vel;

	// set up face steering
	FaceSteering face(mOwnerID, lookPos);
	face.update();
	PhysicsData faceData = face.getData();
	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;

	this->mData = data;
	return this;
}

// NOTE: This function assumes that each vector in inputVectors is normalized.
Vector2D FlockingSteering::weightedBlend(std::vector<Vector2D> inputVectors, std::vector<float> weights)
{
	// ensure the sizes match up
	assert(inputVectors.size() == weights.size());

	Vector2D output;

	for (int i = 0; i < (int)inputVectors.size(); i++)
	{
		output += inputVectors[i] * weights[i];
	}

	return output;
}