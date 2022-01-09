#include <Trackable.h>
#include "Steering.h"

class SeparationSteering : public Steering
{
public:
	SeparationSteering(const UnitID& ownerID);

protected:
	virtual Steering* getSteering();
};