#include <Trackable.h>
#include "Steering.h"
#include "GameMessage.h"
#include "Path.h"

class Path;

class FollowPathSteering : public Steering
{
public:
	FollowPathSteering(const UnitID& ownerID);
	~FollowPathSteering();

	void calculatePath(Vector2D& targetLocation);

protected:
	virtual Steering* getSteering();
	Path mPathToFollow;
	Node* mCurrentNode;
};