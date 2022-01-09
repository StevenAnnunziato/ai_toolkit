#include <cassert>

#include "Steering.h"
#include "FollowPathSteering.h"
#include "SeekSteering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Grid.h"
#include "GridGraph.h"
#include "GridPathfinder.h"
#include "Graph.h"
#include "Node.h"

FollowPathSteering::FollowPathSteering(const UnitID& ownerID)
	: Steering()
{
	mType = Steering::FOLLOW_PATH;
	setOwnerID(ownerID);

	// init path
	Path myPath;
	mPathToFollow = myPath;
	mCurrentNode = NULL;
}
FollowPathSteering::~FollowPathSteering()
{
	
}

Steering* FollowPathSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	// if there is no path, do nothing
	if (mCurrentNode == NULL)
	{
		data.vel = Vector2D();
		data.acc = Vector2D();
		data.rotVel = 0;
		data.rotAcc = 0;
		this->mData = data;
		return this;
	}

	// get this node's position
	mTargetLoc = gpGame->getGrid()->getULCornerOfCell(mCurrentNode->getId());

	if (mCurrentNode != NULL)
	{
		// set up seek steering and move towards current node
		SeekSteering seek(mOwnerID, mTargetLoc, mTargetID);
		seek.update();
		data = seek.getData();
	}
	else
	{
		std::cout << "DONE" << std::endl;
		return nullptr;
	}

	// set up face steering
	FaceSteering face(mOwnerID, mTargetLoc);
	face.update();
	PhysicsData faceData = face.getData();
	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;
	data.maxSpeed = 100;

	// get distance from target
	Vector2D dir = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	float dist = dir.getLengthSquared();

	// if we are close enough, move on to the next node
	const unsigned int THRESHOLD = 1600;
	if (dist <= THRESHOLD)
	{
		mCurrentNode = mPathToFollow.getAndRemoveNextNode();
	}

	this->mData = data;
	return this;
}

// This function delegates to Game::mpPathfinder to calculate a path
void FollowPathSteering::calculatePath(Vector2D& targetLocation)
{
	// find start node
	Vector2D myPos = gpGame->getUnitManager()->getUnit(mOwnerID)->getPositionComponent()->getPosition();
	Node* fromNode = gpGame->getGridGraph()->getNode(gpGame->getGrid()->getCellIndexFromPixelXY((int)myPos.getX(), (int)myPos.getY()));

	// find end node
	Node* toNode = gpGame->getGridGraph()->getNode(gpGame->getGrid()->getCellIndexFromPixelXY((int)targetLocation.getX(), (int)targetLocation.getY()));;

	// get a path
	mPathToFollow = gpGame->getPathfinder()->findPath(toNode, fromNode);

	// set up the first node for the pathfinder
	mCurrentNode = mPathToFollow.getAndRemoveNextNode();
}