#include <Trackable.h>
#include "Steering.h"

class FollowFlowSteering : public Steering
{
public:
	FollowFlowSteering(const UnitID& ownerID);

protected:
	virtual Steering* getSteering();
};