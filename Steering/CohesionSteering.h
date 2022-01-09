#include <Trackable.h>
#include "Steering.h"

class CohesionSteering : public Steering
{
public:
	CohesionSteering(const UnitID& ownerID);

protected:
	virtual Steering* getSteering();
};