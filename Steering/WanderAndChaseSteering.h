#include <Trackable.h>
#include "Steering.h"

class WanderSteering;

class WanderAndChaseSteering : public Steering
{
public:
	WanderAndChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
	~WanderAndChaseSteering();

protected:
	virtual Steering* getSteering();
	WanderSteering* mpWanderSteering;
};